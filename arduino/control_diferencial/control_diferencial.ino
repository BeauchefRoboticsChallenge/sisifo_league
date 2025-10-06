/*
 * Control Diferencial Robot - Sisifo League
 * 
 * Este programa controla un robot diferencial usando dos joysticks unidimensionales (potenciómetros).
 * Cada joystick controla la velocidad de una rueda independientemente.
 * 
 * Conexiones de Hardware:
 * - Joystick Izquierdo: Pin A0 (Potenciómetro para rueda izquierda)
 * - Joystick Derecho: Pin A1 (Potenciómetro para rueda derecha)
 * - Motor Izquierdo: Pines 5 (PWM), 4 (DIR1), 3 (DIR2)
 * - Motor Derecho: Pines 6 (PWM), 7 (DIR1), 8 (DIR2)
 * 
 * Funcionamiento:
 * - Posición central del joystick: Motor detenido
 * - Joystick hacia adelante: Motor gira hacia adelante
 * - Joystick hacia atrás: Motor gira hacia atrás
 * - La velocidad es proporcional a la distancia desde el centro
 * 
 * Autor: Beauchef Robotics Challenge
 * Fecha: Octubre 2025
 */

// ========== CONFIGURACIÓN DE PINES ==========
// Pines analógicos para joysticks
const int PIN_JOYSTICK_IZQ = A0;  // Joystick rueda izquierda
const int PIN_JOYSTICK_DER = A1;  // Joystick rueda derecha

// Pines para motor izquierdo
const int PIN_MOTOR_IZQ_PWM = 5;   // Control de velocidad PWM
const int PIN_MOTOR_IZQ_DIR1 = 4;  // Dirección 1
const int PIN_MOTOR_IZQ_DIR2 = 3;  // Dirección 2

// Pines para motor derecho
const int PIN_MOTOR_DER_PWM = 6;   // Control de velocidad PWM
const int PIN_MOTOR_DER_DIR1 = 7;  // Dirección 1
const int PIN_MOTOR_DER_DIR2 = 8;  // Dirección 2

// ========== CONSTANTES DE CONFIGURACIÓN ==========
const int VALOR_CENTRO = 512;      // Valor central del ADC (10 bits: 0-1023)
const int ZONA_MUERTA = 20;        // Zona muerta alrededor del centro
const int VELOCIDAD_MAXIMA = 255;  // Velocidad máxima PWM (0-255)
const int DELAY_LECTURA = 50;      // Delay entre lecturas (ms)

// ========== VARIABLES GLOBALES ==========
int valorJoystickIzq = 0;   // Valor leído del joystick izquierdo
int valorJoystickDer = 0;   // Valor leído del joystick derecho
int velocidadIzq = 0;       // Velocidad calculada motor izquierdo
int velocidadDer = 0;       // Velocidad calculada motor derecho

void setup() {
  // Inicializar comunicación serie para depuración
  Serial.begin(9600);
  Serial.println("=== Control Diferencial Robot ===");
  Serial.println("Inicializando sistema...");
  
  // Configurar pines de motores como salidas
  pinMode(PIN_MOTOR_IZQ_PWM, OUTPUT);
  pinMode(PIN_MOTOR_IZQ_DIR1, OUTPUT);
  pinMode(PIN_MOTOR_IZQ_DIR2, OUTPUT);
  
  pinMode(PIN_MOTOR_DER_PWM, OUTPUT);
  pinMode(PIN_MOTOR_DER_DIR1, OUTPUT);
  pinMode(PIN_MOTOR_DER_DIR2, OUTPUT);
  
  // Detener motores al inicio
  detenerMotores();
  
  Serial.println("Sistema listo!");
  Serial.println("Formato: JoyIzq | JoyDer | VelIzq | VelDer");
  Serial.println("----------------------------------------------");
}

void loop() {
  // Leer valores de los joysticks
  leerJoysticks();
  
  // Calcular velocidades de los motores
  calcularVelocidades();
  
  // Controlar motores
  controlarMotorIzquierdo(velocidadIzq);
  controlarMotorDerecho(velocidadDer);
  
  // Mostrar información por serial
  mostrarInformacion();
  
  // Pequeña pausa
  delay(DELAY_LECTURA);
}

// ========== FUNCIONES DE LECTURA ==========
void leerJoysticks() {
  valorJoystickIzq = analogRead(PIN_JOYSTICK_IZQ);
  valorJoystickDer = analogRead(PIN_JOYSTICK_DER);
}

// ========== FUNCIONES DE CÁLCULO ==========
void calcularVelocidades() {
  velocidadIzq = mapearVelocidad(valorJoystickIzq);
  velocidadDer = mapearVelocidad(valorJoystickDer);
}

int mapearVelocidad(int valorJoystick) {
  int diferencia = valorJoystick - VALOR_CENTRO;
  
  // Aplicar zona muerta
  if (abs(diferencia) < ZONA_MUERTA) {
    return 0;
  }
  
  // Mapear a velocidad PWM
  if (diferencia > 0) {
    // Hacia adelante
    return map(diferencia, ZONA_MUERTA, VALOR_CENTRO, 0, VELOCIDAD_MAXIMA);
  } else {
    // Hacia atrás (velocidad negativa)
    return map(diferencia, -VALOR_CENTRO, -ZONA_MUERTA, -VELOCIDAD_MAXIMA, 0);
  }
}

// ========== FUNCIONES DE CONTROL DE MOTORES ==========
void controlarMotorIzquierdo(int velocidad) {
  if (velocidad == 0) {
    // Detener motor
    analogWrite(PIN_MOTOR_IZQ_PWM, 0);
    digitalWrite(PIN_MOTOR_IZQ_DIR1, LOW);
    digitalWrite(PIN_MOTOR_IZQ_DIR2, LOW);
  } else if (velocidad > 0) {
    // Girar hacia adelante
    analogWrite(PIN_MOTOR_IZQ_PWM, velocidad);
    digitalWrite(PIN_MOTOR_IZQ_DIR1, HIGH);
    digitalWrite(PIN_MOTOR_IZQ_DIR2, LOW);
  } else {
    // Girar hacia atrás
    analogWrite(PIN_MOTOR_IZQ_PWM, abs(velocidad));
    digitalWrite(PIN_MOTOR_IZQ_DIR1, LOW);
    digitalWrite(PIN_MOTOR_IZQ_DIR2, HIGH);
  }
}

void controlarMotorDerecho(int velocidad) {
  if (velocidad == 0) {
    // Detener motor
    analogWrite(PIN_MOTOR_DER_PWM, 0);
    digitalWrite(PIN_MOTOR_DER_DIR1, LOW);
    digitalWrite(PIN_MOTOR_DER_DIR2, LOW);
  } else if (velocidad > 0) {
    // Girar hacia adelante
    analogWrite(PIN_MOTOR_DER_PWM, velocidad);
    digitalWrite(PIN_MOTOR_DER_DIR1, HIGH);
    digitalWrite(PIN_MOTOR_DER_DIR2, LOW);
  } else {
    // Girar hacia atrás
    analogWrite(PIN_MOTOR_DER_PWM, abs(velocidad));
    digitalWrite(PIN_MOTOR_DER_DIR1, LOW);
    digitalWrite(PIN_MOTOR_DER_DIR2, HIGH);
  }
}

void detenerMotores() {
  controlarMotorIzquierdo(0);
  controlarMotorDerecho(0);
}

// ========== FUNCIONES DE DEPURACIÓN ==========
void mostrarInformacion() {
  Serial.print(valorJoystickIzq);
  Serial.print(" | ");
  Serial.print(valorJoystickDer);
  Serial.print(" | ");
  Serial.print(velocidadIzq);
  Serial.print(" | ");
  Serial.println(velocidadDer);
}