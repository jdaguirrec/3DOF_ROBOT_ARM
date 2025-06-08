#include <Servo.h>
#include <math.h>

Servo servo1;  // theta1 -> pin 10
Servo servo2;  // theta2 -> pin 9
Servo servo3;  // theta3 -> pin 6

const float L1 = 10.4;
const float L2 = 10.4;

const int steps = 100;
float t_values[steps];
float x_values[steps];
float y_values[steps];
float z_values[steps];

int currentStep = 0;
unsigned long lastUpdate = 0;
const unsigned long interval = 50; // milisegundos entre pasos

void setup() {
  servo1.attach(10);
  servo2.attach(9);
  servo3.attach(6);

  // Precalcular los valores de la trayectoria
  for (int i = 0; i < steps; i++) {
    float t = PI * i / (steps - 1);
    t_values[i] = t;
    x_values[i] = 9 * cos(3*t);
    y_values[i] = 15 * sin(t);
    z_values[i] = 8 + 5*sin(10 * t);
  }
}

void loop() {
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();

    float x = x_values[currentStep];
    float y = y_values[currentStep];
    float z = z_values[currentStep];

    float theta1 = calcular_theta1(x, y, z, L1, L2);
    float theta2 = calcular_theta2(x, y, z, L1, L2);
    float theta3 = calcular_theta3(x, y);

    // Clamp ángulos entre 0 y 180 grados
    theta1 = constrain(theta1, 0, 180);
    theta2 = constrain(theta2, 0, 180);
    theta3 = constrain(theta3, 0, 180);

    // Enviar ángulos a los servos
    servo1.write(theta1);
    servo2.write(theta2);
    servo3.write(theta3);

    currentStep++;
    if (currentStep >= steps) {
      currentStep = 0; // reiniciar ciclo
    }
  }
}

// Función para calcular theta1
float calcular_theta1(float x, float y, float z, float L1, float L2) {
  float base = sqrt(x * x + y * y);
  float distancia_total = sqrt(x * x + y * y + z * z);

  float angulo1 = atan2(z, base);
  float numerador = L1 * L1 + x * x + y * y + z * z - L2 * L2;
  float denominador = 2 * L1 * distancia_total;
  float angulo2 = acos(numerador / denominador);

  float theta1 = angulo1 + angulo2;
  return degrees(theta1);
}

// Función para calcular theta2
float calcular_theta2(float x, float y, float z, float L1, float L2) {
  float numerador = x * x + y * y + z * z - L1 * L1 - L2 * L2;
  float denominador = 2 * L1 * L2;
  float theta2 = -acos(numerador / denominador);
  return 180 + degrees(theta2);
}

// Función para calcular theta3
float calcular_theta3(float x, float y) {
  float base = sqrt(x * x + y * y);
  float theta3 = acos(x / base);
  return degrees(theta3);
}
