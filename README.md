# Sisifo League - BRC25+

Liga Sisifo para Beauchef Robotics Challenge 2025+

## Descripción

Este repositorio contiene el código y recursos para robots de la Liga Sisifo, enfocada en robots diferenciales controlados por joysticks digitales de 3 estados (implementados con 2 switches por joystick).

## Estructura del Proyecto

```
sisifo_league/
├── arduino/                    # Código Arduino
│   └── control_diferencial/    # Control diferencial con joysticks
│       └── control_diferencial.ino
├── README.md
└── LICENSE
```

## Control Diferencial Arduino

### Características

- Control de robot diferencial con dos ruedas independientes
- Control mediante joysticks digitales de 3 estados: UP, DOWN, y NEUTRAL (implementados con 2 switches por joystick)
- Cada joystick controla una rueda de forma independiente
- Estado NEUTRAL se aplica automáticamente cuando ningún switch está activado
- Aceleración gradual al cambiar de NEUTRAL a UP/DOWN
- Parada instantánea al volver a NEUTRAL
- Comunicación serie para depuración

### Hardware Requerido

- Arduino Uno/Nano o compatible
- 2 Joysticks digitales de 3 estados (cada uno con 2 switches: UP y DOWN que se conectan a GND cuando activos)
- 2 Motores DC con driver (ej: L298N)
- Fuente de alimentación adecuada para los motores
- Cables de conexión

### Conexiones

#### Joysticks Digitales
**Joystick Izquierdo (2 switches):**
- Switch UP: Pin 9
- Switch DOWN: Pin 10

**Joystick Derecho (2 switches):**
- Switch UP: Pin 11
- Switch DOWN: Pin 12

**Notas importantes:**
- Los pines utilizan resistencias pullup internas del Arduino
- Los switches deben conectar el pin correspondiente a GND cuando están activos (lógica invertida)
- Cuando ningún switch está activo (ambos en HIGH), el estado es NEUTRAL
- No se requieren resistencias externas

#### Motores
**Motor Izquierdo:**
- PWM: Pin 5
- Dirección 1: Pin 4
- Dirección 2: Pin 3

**Motor Derecho:**
- PWM: Pin 6
- Dirección 1: Pin 7
- Dirección 2: Pin 8

### Uso

1. Cargar el código `control_diferencial.ino` en el Arduino
2. Conectar el hardware según el esquema
3. Abrir el Monitor Serie (9600 baudios) para ver información de depuración
4. Controlar el robot con los joysticks:
   - **NEUTRAL** (ningún switch activo): Robot detenido (velocidad = 0)
   - **NEUTRAL → UP** (activar switch UP): La rueda acelera gradualmente hacia adelante
   - **UP → NEUTRAL** (soltar switch UP): La rueda se detiene instantáneamente
   - **NEUTRAL → DOWN** (activar switch DOWN): La rueda acelera gradualmente en reversa
   - **DOWN → NEUTRAL** (soltar switch DOWN): La rueda se detiene instantáneamente

### Configuración

El código incluye constantes configurables en la parte superior:

```cpp
const int VELOCIDAD_MAXIMA = 255;    // Velocidad máxima PWM (0-255)
const int DELAY_LECTURA = 50;        // Delay entre lecturas (ms)
const int ACELERACION = 5;           // Incremento de velocidad por ciclo
const float TIEMPO_ACELERACION = 2.0; // Tiempo en segundos para alcanzar velocidad máxima
```

**Ajuste de aceleración:**
- `ACELERACION`: Define cuánto aumenta la velocidad en cada ciclo del loop
- Con `DELAY_LECTURA = 50ms` y `ACELERACION = 5`, se alcanza velocidad máxima en ~2.5 segundos
- Para aceleración más rápida, incrementar `ACELERACION`
- Para aceleración más suave, reducir `ACELERACION`

### Funcionamiento

El programa:
1. Lee el estado de los 2 switches de cada joystick (UP y DOWN)
2. Determina el estado del joystick: UP (switch UP activo), DOWN (switch DOWN activo), o NEUTRAL (ningún switch activo)
3. Actualiza la velocidad objetivo según el estado
4. Aplica aceleración gradual hasta alcanzar la velocidad objetivo
5. Detiene instantáneamente los motores al volver a NEUTRAL
6. Controla cada motor independientemente según su joystick
7. Envía información de depuración por puerto serie

**Comportamiento detallado:**
- Al presionar UP/DOWN desde NEUTRAL, la velocidad incrementa gradualmente
- Mientras UP/DOWN se mantiene presionado, la velocidad aumenta hasta el máximo
- Al soltar (volver a NEUTRAL), el motor se detiene inmediatamente sin rampa

## Contribuciones

Las contribuciones son bienvenidas. Por favor, crea un pull request o abre un issue para discutir cambios.

## Licencia

Ver archivo `LICENSE` para más información.
