# Sisifo League - BRC25+

Liga Sisifo para Beauchef Robotics Challenge 2025+

## Descripción

Este repositorio contiene el código y recursos para robots de la Liga Sisifo, enfocada en robots diferenciales controlados por joysticks.

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
- Control mediante dos joysticks unidimensionales (potenciómetros)
- Cada joystick controla una rueda de forma independiente
- Zona muerta configurable para evitar movimientos no deseados
- Comunicación serie para depuración

### Hardware Requerido

- Arduino Uno/Nano o compatible
- 2 Potenciómetros (joysticks unidimensionales)
- 2 Motores DC con driver (ej: L298N)
- Fuente de alimentación adecuada para los motores
- Cables de conexión

### Conexiones

#### Joysticks (Potenciómetros)
- Joystick Izquierdo: Pin A0
- Joystick Derecho: Pin A1
- VCC: 5V
- GND: GND

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
4. Mover los joysticks para controlar el robot:
   - Posición central: Robot detenido
   - Joystick hacia adelante: Rueda gira hacia adelante
   - Joystick hacia atrás: Rueda gira hacia atrás
   - La velocidad es proporcional a la distancia del centro

### Configuración

El código incluye constantes configurables en la parte superior:

```cpp
const int ZONA_MUERTA = 20;        // Zona muerta alrededor del centro
const int VELOCIDAD_MAXIMA = 255;  // Velocidad máxima PWM
const int DELAY_LECTURA = 50;      // Delay entre lecturas (ms)
```

### Funcionamiento

El programa:
1. Lee los valores analógicos de ambos joysticks
2. Mapea estos valores a velocidades de motor (con zona muerta)
3. Controla cada motor independientemente según su joystick
4. Envía información de depuración por puerto serie

## Contribuciones

Las contribuciones son bienvenidas. Por favor, crea un pull request o abre un issue para discutir cambios.

## Licencia

Ver archivo `LICENSE` para más información.
