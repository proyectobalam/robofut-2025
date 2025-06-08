/*
  ============================================================
   PROYECTO BALAM 2025 – Robofut - motor_test
   Universidad Galileo | Grupo Intelecto
  ============================================================

   Descripción:
     Fase: COMPITE.
     Ejemplo básico de movimiento de motores

   Objetivo:
     Hacer test de movimiento de todos los motores

   Librerías:
    - TB6612_ESP32
   Autor(es):
     Mateo García - Grupo Intelecto
     Equipo BALAM / Grupo Intelecto

   Fecha: Junio 2025
   Versión: 2.0

   Organización: https://github.com/proyectobalam
   Repositorio: https://github.com/proyectobalam/robofut-2025
  ============================================================
*/

// ------------------------ Definición de Pines ------------------------
// Motor A (motor frontal izquierdo)
int PWMA  = 15; // Pin PWM para controlar velocidad del Motor A
int AIN1  = 18; // Pin digital para dirección A del Motor A
int AIN2  = 5;  // Pin digital para dirección B del Motor A

// Motor A1 (motor trasero izquierdo)
int PWMA1 = 12; // Pin PWM para controlar velocidad del Motor A1
int AIN11 = 32; // Pin digital para dirección A del Motor A1
int AIN22 = 33; // Pin digital para dirección B del Motor A1

// Motor B (motor frontal derecho)
int PWMB  = 2;  // Pin PWM para controlar velocidad del Motor B
int BIN1  = 27; // Pin digital para dirección A del Motor B
int BIN2  = 14; // Pin digital para dirección B del Motor B

// Motor B1 (motor trasero derecho)
int PWMB1 = 13; // Pin PWM para controlar velocidad del Motor B1
int BIN11 = 25; // Pin digital para dirección A del Motor B1
int BIN22 = 26; // Pin digital para dirección B del Motor B1

// ---------------------- Configuración Inicial ----------------------
void setup() {
  // Configurar todos los pines como salida
  pinMode(PWMA,   OUTPUT);
  pinMode(AIN1,   OUTPUT);
  pinMode(AIN2,   OUTPUT);
  pinMode(PWMA1,  OUTPUT);
  pinMode(AIN11,  OUTPUT);
  pinMode(AIN22,  OUTPUT);
  pinMode(PWMB,   OUTPUT);
  pinMode(BIN1,   OUTPUT);
  pinMode(BIN2,   OUTPUT);
  pinMode(PWMB1,  OUTPUT);
  pinMode(BIN11,  OUTPUT);
  pinMode(BIN22,  OUTPUT);

  // Iniciar comunicación serial para depuración
  Serial.begin(115200);
}

// ------------------------- Bucle Principal -------------------------
void loop() {
  // ----------------------- Encender Motores -----------------------
  // Activar la señal PWM para alimentar los motores
  digitalWrite(PWMA,  HIGH);
  digitalWrite(PWMB,  HIGH);
  digitalWrite(PWMA1, HIGH);
  digitalWrite(PWMB1, HIGH);

  // ----------------------- Movimiento Adelante -----------------------
  // AINx = PWM value, AINy = 0 para girar en una dirección
  analogWrite(AIN1, 250);  // Motor A hacia adelante
  analogWrite(AIN2,   0);
  analogWrite(BIN1, 250);  // Motor B hacia adelante
  analogWrite(BIN2,   0);
  analogWrite(AIN11,250);  // Motor A1 hacia adelante
  analogWrite(AIN22, 0);
  analogWrite(BIN11,250);  // Motor B1 hacia adelante
  analogWrite(BIN22, 0);
  Serial.println("Adelante");
  delay(5000); // Mantener movimiento por 5 segundos

  // ----------------------- Movimiento Atrás -----------------------
  // Invertir señales de dirección
  analogWrite(AIN1,   0);
  analogWrite(AIN2, 250);
  analogWrite(BIN1,   0);
  analogWrite(BIN2, 250);
  analogWrite(AIN11,  0);
  analogWrite(AIN22,250);
  analogWrite(BIN11,  0);
  analogWrite(BIN22,250);
  Serial.println("Atrás");
  delay(2000); // Mantener por 2 segundos

  // ----------------------- Giro a la Derecha -----------------------
  // Solo motores izquierdos avanzan
  analogWrite(AIN1, 250);
  analogWrite(AIN2,   0);
  analogWrite(BIN1,   0);
  analogWrite(BIN2,250);
  analogWrite(AIN11,250);
  analogWrite(AIN22, 0);
  analogWrite(BIN11, 0);
  analogWrite(BIN22,250);
  Serial.println("Derecha");
  delay(2000);

  // ----------------------- Giro a la Izquierda -----------------------
  // Solo motores derechos avanzan
  analogWrite(AIN1,   0);
  analogWrite(AIN2,250);
  analogWrite(BIN1,250);
  analogWrite(BIN2,  0);
  analogWrite(AIN11,  0);
  analogWrite(AIN22,250);
  analogWrite(BIN11,250);
  analogWrite(BIN22,  0);
  Serial.println("Izquierda");
  delay(2000);
}

