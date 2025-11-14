/*
 * Control Diferencial Robot - Sisifo League
 * 
 * Este programa controla un robot diferencial usando dos joysticks digitales de 3 estados.
 * Cada joystick tiene tres señales digitales (UP, DOWN, NEUTRAL) que controlan una rueda.
 * Solo una señal puede estar activa a la vez por restricción de hardware.
 * 
 * Los pines de entrada utilizan resistencias pulldown internas, eliminando la necesidad
 * de resistencias externas. Requiere una placa compatible con INPUT_PULLDOWN (ej: ESP32).
 * 
 * Conexiones de Hardware:
 * - Joystick Izquierdo UP: Pin 9 (señal digital para arriba)
 * - Joystick Izquierdo DOWN: Pin 10 (señal digital para abajo)
 * - Joystick Izquierdo NEUTRAL: Pin 11 (señal digital para neutral)
 * - Joystick Derecho UP: Pin 12 (señal digital para arriba)
 * - Joystick Derecho DOWN: Pin A2 (señal digital para abajo)
 * - Joystick Derecho NEUTRAL: Pin A3 (señal digital para neutral)
 * - Motor Izquierdo: Pines 5 (PWM), 4 (DIR1), 3 (DIR2)
 * - Motor Derecho: Pines 6 (PWM), 7 (DIR1), 8 (DIR2)
 * 
 * Funcionamiento:
 * - NEUTRAL: Motor detenido (velocidad = 0)
 * - NEUTRAL → UP: Velocidad aumenta gradualmente hasta el máximo
 * - UP → NEUTRAL: Motor se detiene instantáneamente
 * - NEUTRAL → DOWN: Velocidad aumenta gradualmente en reversa hasta el máximo
 * - DOWN → NEUTRAL: Motor se detiene instantáneamente
 * 
 * Autor: Beauchef Robotics Challenge
 * Fecha: Noviembre 2025
 */

// ========== CONFIGURACIÓN DE PINES ==========
// Pines digitales para joystick izquierdo
const int PIN_JOY_IZQ_UP = 9;       // Señal UP izquierda
const int PIN_JOY_IZQ_DOWN = 10;    // Señal DOWN izquierda
const int PIN_JOY_IZQ_NEUTRAL = 11; // Señal NEUTRAL izquierda

// Pines digitales para joystick derecho
const int PIN_JOY_DER_UP = 12;      // Señal UP derecha
const int PIN_JOY_DER_DOWN = A2;    // Señal DOWN derecha
const int PIN_JOY_DER_NEUTRAL = A3; // Señal NEUTRAL derecha

// Pines para motor izquierdo
const int PIN_MOTOR_IZQ_PWM = 5;   // Control de velocidad PWM
const int PIN_MOTOR_IZQ_DIR1 = 4;  // Dirección 1
const int PIN_MOTOR_IZQ_DIR2 = 3;  // Dirección 2

// Pines para motor derecho
const int PIN_MOTOR_DER_PWM = 6;   // Control de velocidad PWM
const int PIN_MOTOR_DER_DIR1 = 7;  // Dirección 1
const int PIN_MOTOR_DER_DIR2 = 8;  // Dirección 2

// ========== CONSTANTES DE CONFIGURACIÓN ==========
const int VELOCIDAD_MAXIMA = 255;    // Velocidad máxima PWM (0-255)
const int DELAY_LECTURA = 50;        // Delay entre lecturas (ms)
const int ACELERACION = 5;           // Incremento de velocidad por ciclo
const float TIEMPO_ACELERACION = 2.0; // Tiempo en segundos para alcanzar velocidad máxima

// Estados del joystick
enum EstadoJoystick {
  ESTADO_NEUTRAL,
  ESTADO_UP,
  ESTADO_DOWN
};

// ========== VARIABLES GLOBALES ==========
// Estado actual de cada joystick
EstadoJoystick estadoIzq = ESTADO_NEUTRAL;
EstadoJoystick estadoDer = ESTADO_NEUTRAL;

// Velocidad actual de cada motor (puede ser negativa)
int velocidadActualIzq = 0;
int velocidadActualDer = 0;

// Velocidad objetivo según el estado del joystick
int velocidadObjetivoIzq = 0;
int velocidadObjetivoDer = 0;

void setup() {
  // Inicializar comunicación serie para depuración
  Serial.begin(9600);
  Serial.println("=== Control Diferencial Robot - 3-State Joysticks ===");
  Serial.println("Inicializando sistema...");
  
  // Configurar pines de joysticks como entradas con resistencias pulldown internas
  pinMode(PIN_JOY_IZQ_UP, INPUT_PULLDOWN);
  pinMode(PIN_JOY_IZQ_DOWN, INPUT_PULLDOWN);
  pinMode(PIN_JOY_IZQ_NEUTRAL, INPUT_PULLDOWN);
  pinMode(PIN_JOY_DER_UP, INPUT_PULLDOWN);
  pinMode(PIN_JOY_DER_DOWN, INPUT_PULLDOWN);
  pinMode(PIN_JOY_DER_NEUTRAL, INPUT_PULLDOWN);
  
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
  Serial.println("Formato: EstadoIzq | EstadoDer | VelIzq | VelDer");
  Serial.println("----------------------------------------------");
}

void loop() {
  // Leer estados de los joysticks
  leerEstadosJoysticks();
  
  // Actualizar velocidades según el estado
  actualizarVelocidades();
  
  // Controlar motores
  controlarMotorIzquierdo(velocidadActualIzq);
  controlarMotorDerecho(velocidadActualDer);
  
  // Mostrar información por serial
  mostrarInformacion();
  
  // Pequeña pausa
  delay(DELAY_LECTURA);
}

// ========== FUNCIONES DE LECTURA ==========
void leerEstadosJoysticks() {
  // Leer estado del joystick izquierdo
  if (digitalRead(PIN_JOY_IZQ_UP) == HIGH) {
    if (estadoIzq != ESTADO_UP) {
      estadoIzq = ESTADO_UP;
      velocidadObjetivoIzq = VELOCIDAD_MAXIMA;
    }
  } else if (digitalRead(PIN_JOY_IZQ_DOWN) == HIGH) {
    if (estadoIzq != ESTADO_DOWN) {
      estadoIzq = ESTADO_DOWN;
      velocidadObjetivoIzq = -VELOCIDAD_MAXIMA;
    }
  } else if (digitalRead(PIN_JOY_IZQ_NEUTRAL) == HIGH) {
    if (estadoIzq != ESTADO_NEUTRAL) {
      estadoIzq = ESTADO_NEUTRAL;
      velocidadObjetivoIzq = 0;
      velocidadActualIzq = 0;  // Parada instantánea
    }
  }
  
  // Leer estado del joystick derecho
  if (digitalRead(PIN_JOY_DER_UP) == HIGH) {
    if (estadoDer != ESTADO_UP) {
      estadoDer = ESTADO_UP;
      velocidadObjetivoDer = VELOCIDAD_MAXIMA;
    }
  } else if (digitalRead(PIN_JOY_DER_DOWN) == HIGH) {
    if (estadoDer != ESTADO_DOWN) {
      estadoDer = ESTADO_DOWN;
      velocidadObjetivoDer = -VELOCIDAD_MAXIMA;
    }
  } else if (digitalRead(PIN_JOY_DER_NEUTRAL) == HIGH) {
    if (estadoDer != ESTADO_NEUTRAL) {
      estadoDer = ESTADO_NEUTRAL;
      velocidadObjetivoDer = 0;
      velocidadActualDer = 0;  // Parada instantánea
    }
  }
}

// ========== FUNCIONES DE CÁLCULO ==========
void actualizarVelocidades() {
  // Actualizar velocidad izquierda
  velocidadActualIzq = actualizarVelocidad(velocidadActualIzq, velocidadObjetivoIzq);
  
  // Actualizar velocidad derecha
  velocidadActualDer = actualizarVelocidad(velocidadActualDer, velocidadObjetivoDer);
}

int actualizarVelocidad(int velocidadActual, int velocidadObjetivo) {
  // Si estamos en neutral, la velocidad ya está en 0
  if (velocidadObjetivo == 0) {
    return 0;
  }
  
  // Si no hemos alcanzado la velocidad objetivo, incrementar gradualmente
  if (velocidadActual < velocidadObjetivo) {
    velocidadActual += ACELERACION;
    if (velocidadActual > velocidadObjetivo) {
      velocidadActual = velocidadObjetivo;
    }
  } else if (velocidadActual > velocidadObjetivo) {
    velocidadActual -= ACELERACION;
    if (velocidadActual < velocidadObjetivo) {
      velocidadActual = velocidadObjetivo;
    }
  }
  
  return velocidadActual;
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
  // Mostrar estado izquierdo
  Serial.print(obtenerNombreEstado(estadoIzq));
  Serial.print(" | ");
  
  // Mostrar estado derecho
  Serial.print(obtenerNombreEstado(estadoDer));
  Serial.print(" | ");
  
  // Mostrar velocidad izquierda
  Serial.print(velocidadActualIzq);
  Serial.print(" | ");
  
  // Mostrar velocidad derecha
  Serial.println(velocidadActualDer);
}

String obtenerNombreEstado(EstadoJoystick estado) {
  switch (estado) {
    case ESTADO_NEUTRAL:
      return "NEUTRAL";
    case ESTADO_UP:
      return "UP     ";
    case ESTADO_DOWN:
      return "DOWN   ";
    default:
      return "UNKNOWN";
  }
}