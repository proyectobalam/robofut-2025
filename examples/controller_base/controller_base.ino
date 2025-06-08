/*
  ============================================================
   PROYECTO BALAM 2025 – Robofut - controllers_base
   Universidad Galileo | Grupo Intelecto
  ============================================================

   Descripción:
     Fase: COMPITE.
     Control de movimiento de robot con cuatro motores DC
     mediante diferentes controles genéricos. Controles probados:
      - Control Marvo para PS4
      - Control GameSir Nova Lite Control Inalámbrico
      - PS4

   Objetivo:
     Controlar el robot en direcciones básicas (adelante, atrás,
     izquierda, derecha y detener) utilizando el joystick derecho.

   Librerías:
    - TB6612_ESP32
    - Bluepad32
      https://github.com/ricardoquesada/bluepad32/

   Autor(es):
     Nesthor Guillermo - Grupo Intelecto
     Gabriela Subuyú – Tesla Lab
     Equipo BALAM / Grupo Intelecto

   Fecha: Mayo 2025
   Versión: 1.5

   Organización: https://github.com/proyectobalam
   Repositorio: https://github.com/proyectobalam/robofut-2025
   Tutorial/Referencia: https://racheldebarros.com/esp32-projects/connect-your-game-controller-to-an-esp32/
  ============================================================
*/

// ------------------------- Librerías -------------------------
// Incluye las librerías para controlar los motores y el gamepad.
#include <TB6612_ESP32.h>
#include <Bluepad32.h>

// ------------------------ Definición de Pines ------------------------
// Cada motor requiere dos pines para dirección y uno para velocidad (PWM).
#define PWM1 15  // Velocidad Motor 1
#define M1_1 18  // Motor 1, entrada A
#define M1_2 5   // Motor 1, entrada B

#define PWM2 2   // Velocidad Motor 2
#define M2_1 27  // Motor 2, entrada A
#define M2_2 14  // Motor 2, entrada B

#define PWM3 12  // Velocidad Motor 3
#define M3_1 33  // Motor 3, entrada A
#define M3_2 32  // Motor 3, entrada B

#define PWM4 13  // Velocidad Motor 4
#define M4_1 26  // Motor 4, entrada A
#define M4_2 25  // Motor 4, entrada B

#define STBY 0    // Pin Standby común para motores 1 y 2
#define STBY1 22  // Pin Standby común para motores 3 y 4

// ---------------------- Parámetros de Motor ----------------------
// Ajusta la orientación de cada motor (1 = normal, -1 = invertido).
const int offsetA = 1;
const int offsetB = 1;
const int offsetA1 = 1;
const int offsetB1 = 1;

// Dirección Bluetooth permitida (6 bytes). Sólo este control podrá conectarse.
// NOTA: Es necesario que coloques la dirección de tu control aquí
const uint8_t allowedAddress[6] = { 0xA4, 0x15, 0x66, 0xC9, 0xBB, 0x40 };

// ---------------------- Inicializar Motores ----------------------
// Crea objetos Motor con sus pines, orientación, stand-by y velocidad PWM.
Motor motor1 = Motor(M1_1, M1_2, PWM1, offsetA, STBY, 5000, 8, 1);
Motor motor2 = Motor(M2_1, M2_2, PWM2, offsetB, STBY, 5000, 8, 2);
Motor motor3 = Motor(M3_1, M3_2, PWM3, offsetA1, STBY1, 5000, 8, 4);
Motor motor4 = Motor(M4_1, M4_2, PWM4, offsetB1, STBY1, 5000, 8, 5);

// ------------------- Variables de Joystick -------------------
// Guardan la última lectura de cada eje (inicialmente en 0).
int LStickY = 0, LStickX = 0;
int RStickY = 0, RStickX = 0;

// ------------- Velocidad Máxima y Deadzone -------------
// Limita la velocidad y evita lecturas muy pequeñas juntas.
int valorMaxMotorL = 255;
int valorMaxMotorR = 255;
const int deadMin = -5;
const int deadMax = 5;

// ------------ Control de Múltiples Controles ------------
// Array para guardar hasta 4 controles conectados simultáneamente.
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// ------------ Callback: Conexión de Controles ------------
// Se ejecuta cuando un control Bluetooth se conecta.
void onConnectedController(ControllerPtr ctl) {
  // Obtener la dirección Bluetooth del control.
  ControllerProperties props = ctl->getProperties();
  const uint8_t* addr = props.btaddr;

  // Verificar si el control está en la lista permitida.
  bool isAllowed = true;
  for (int i = 0; i < 6; i++) {
    if (addr[i] != allowedAddress[i]) {
      isAllowed = false;
      break;
    }
  }
  if (!isAllowed) {
    Serial.println("Control rechazado: dirección no permitida.");
    return;
  }

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      Serial.printf("Control conectado en ranura %d\n", i);
      return;
    }
  }
  Serial.println("Control conectado, pero no hay ranuras libres.");
}

// ------------ Callback: Desconexión de Control ------------
// Se ejecuta cuando un control Bluetooth se desconecta.
void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      Serial.printf("Control desconectado de ranura %d\n", i);
      return;
    }
  }
  Serial.println("Se desconectó un control no registrado.");
}

// ------------ Ver Valores del Control ------------
// Imprime ejes, botones y sensores en el monitor serial.
void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad=0x%02x, buttons=0x%04x, AX=%4d AY=%4d RX=%4d RY=%4d\n",
    ctl->index(), ctl->dpad(), ctl->buttons(),
    ctl->axisX(), ctl->axisY(), ctl->axisRX(), ctl->axisRY());
}

// ------------ Procesar Entrada del Contorl ------------
// Lee botones y joysticks para accionar motores.
// AQUI DEBES AGREGAR TU CÓDIGO DE MOVIMIENTO DE MOTORES
void processGamepad(ControllerPtr ctl) {

  // Cambiar velocidad máx. con botón R2 (0x0080).
  if (ctl->buttons() == 0x0080) {
    valorMaxMotorL = (valorMaxMotorL == 255) ? 200 : 255;
    valorMaxMotorR = valorMaxMotorL;
    Serial.printf("Velocidad máxima: %d\n", valorMaxMotorL);
  }

  // Botones básicos para activar motores 1-4:
  if (ctl->buttons() & 0x0001) {
    motor1.drive(200, 5);  // X
  }
  if (ctl->buttons() & 0x0004) {
    motor2.drive(200, 5);  // Square
  }
  if (ctl->buttons() & 0x0008) {
    motor3.drive(200, 5);  // Triangle
  }
  if (ctl->buttons() & 0x0002){ 
    motor4.drive(200, 5);  // Triangle
  }  ≈

  // Joystick derecho para movimiento:
  int ejeX = ctl->axisRX();  // Horizontal
  int ejeY = ctl->axisRY();  // Vertical
  // Aplicar deadzone para evitar ruido:
  ejeX = (abs(ejeX) < deadMax) ? 0 : ejeX;
  ejeY = (abs(ejeY) < deadMax) ? 0 : ejeY;

  // Mapear valores del joystick a velocidad de motor:
  int velocidad = map(ejeY, 512, -508, -valorMaxMotorL, valorMaxMotorL);
  int giro = map(ejeX, -508, 512, -valorMaxMotorR, valorMaxMotorR);
  float factorGiro = 0.6;  // Suaviza el giro

  int velIzq = constrain(velocidad + giro * factorGiro, -255, 255);
  int velDer = constrain(velocidad - giro * factorGiro, -255, 255);

  // Enviar comandos a los motores 
  motor2.drive(velIzq, 5);
  motor4.drive(velDer, 5);

  // Frenar si no hay movimiento:
  if (velocidad == 0 && giro == 0) {
    brake(motor1, motor2);
    brake(motor3, motor4);
  }
}

// ------------ Procesar Todos los Controles ------------
void processControllers() {
  // Revisar cada control conectado y procesar su entrada si hay datos.
  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected() && ctl->hasData()) {
      processGamepad(ctl);
    }
  }
}

// ---------------------- Setup de Arduino ----------------------
void setup() {
  Serial.begin(115200);  // Iniciar monitor serial

  // Frenar todos los motores al iniciar:
  // NOTA: Esta línea de código es importante, no la quites
  brake(motor1, motor2);
  brake(motor3, motor4);

  // Mostrar versión de firmware Bluetooth:
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  BP32.setup(&onConnectedController, &onDisconnectedController);

  BP32.enableVirtualDevice(false);
}

// ---------------------- Loop Principal ----------------------
void loop() {
  // Leer datos de todos los controles (actualiza estado interno):
  if (BP32.update()) {
    processControllers();
  }

  // Pequeño retraso para no sobrecargar el watchdog:
  delay(150);
}
