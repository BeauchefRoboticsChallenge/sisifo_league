# Procedimiento de Prueba - Control Diferencial con Joysticks de 3 Estados

## Configuración de Hardware para Pruebas

### Opción 1: Prueba con Switches/Botones Temporales
Si no tienes los joysticks de 3 estados, puedes probar con switches o botones:

1. **Por cada rueda, conecta 3 botones/switches:**
   - Botón UP conectado al pin correspondiente y GND
   - Botón DOWN conectado al pin correspondiente y GND
   - Botón NEUTRAL conectado al pin correspondiente y GND

2. **Configuración de pines:**
   - Rueda Izquierda: UP(9), DOWN(10), NEUTRAL(11)
   - Rueda Derecha: UP(12), DOWN(A2), NEUTRAL(A3)

3. **IMPORTANTE:** Solo presiona un botón a la vez por rueda

### Opción 2: Simulación con Código de Prueba
Modifica temporalmente el código para simular estados en el Monitor Serie

## Casos de Prueba

### Prueba 1: Neutral → UP → Neutral (Rueda Izquierda)
**Objetivo:** Verificar aceleración gradual y parada instantánea

1. Conecta el Monitor Serie (9600 baudios)
2. Presiona el botón NEUTRAL izquierdo
3. Verifica en el monitor: `NEUTRAL | ... | 0 | ...`
4. Presiona el botón UP izquierdo
5. Observa en el monitor: La velocidad debe incrementar gradualmente:
   ```
   UP      | ... | 5   | ...
   UP      | ... | 10  | ...
   UP      | ... | 15  | ...
   ...
   UP      | ... | 250 | ...
   UP      | ... | 255 | ...
   ```
6. Suelta UP y presiona NEUTRAL
7. Verifica: `NEUTRAL | ... | 0 | ...` (cambio instantáneo)

**Resultado Esperado:**
- ✓ Velocidad aumenta gradualmente (incrementos de 5 cada 50ms)
- ✓ Alcanza velocidad máxima (255)
- ✓ Al volver a NEUTRAL, velocidad = 0 instantáneamente

### Prueba 2: Neutral → DOWN → Neutral (Rueda Izquierda)
**Objetivo:** Verificar aceleración gradual en reversa

1. Presiona el botón NEUTRAL izquierdo
2. Verifica: `NEUTRAL | ... | 0 | ...`
3. Presiona el botón DOWN izquierdo
4. Observa velocidades negativas incrementando:
   ```
   DOWN    | ... | -5   | ...
   DOWN    | ... | -10  | ...
   DOWN    | ... | -15  | ...
   ...
   DOWN    | ... | -255 | ...
   ```
5. Suelta DOWN y presiona NEUTRAL
6. Verifica parada instantánea: `NEUTRAL | ... | 0 | ...`

**Resultado Esperado:**
- ✓ Velocidad disminuye gradualmente (incrementos negativos)
- ✓ Alcanza velocidad máxima en reversa (-255)
- ✓ Parada instantánea al volver a NEUTRAL

### Prueba 3: Independencia de Ruedas
**Objetivo:** Verificar que cada rueda se controla independientemente

1. Rueda izquierda en NEUTRAL, rueda derecha en NEUTRAL
2. Verifica: `NEUTRAL | NEUTRAL | 0 | 0`
3. Presiona UP izquierdo (mantener), NEUTRAL derecho
4. Observa: Solo velocidad izquierda aumenta
   ```
   UP      | NEUTRAL | 50  | 0
   UP      | NEUTRAL | 100 | 0
   ```
5. Ahora presiona DOWN derecho (mantener ambos)
6. Observa: Izquierda sigue aumentando, derecha disminuye
   ```
   UP      | DOWN    | 150 | -50
   UP      | DOWN    | 200 | -100
   ```

**Resultado Esperado:**
- ✓ Cada rueda se controla independientemente
- ✓ Ambas pueden estar en estados diferentes simultáneamente

### Prueba 4: Tiempo de Aceleración
**Objetivo:** Verificar que el tiempo de aceleración es apropiado

1. Con cronómetro, medir tiempo desde NEUTRAL → UP hasta velocidad máxima
2. **Cálculo esperado:**
   - Velocidad máxima = 255
   - Aceleración = 5 por ciclo
   - Delay = 50ms por ciclo
   - Ciclos necesarios = 255 / 5 = 51 ciclos
   - Tiempo = 51 × 50ms = 2.55 segundos

**Resultado Esperado:**
- ✓ Tiempo de aceleración ~2.5 segundos (puede variar ligeramente)

### Prueba 5: Comportamiento de Motores
**Objetivo:** Verificar que los motores responden correctamente

1. Con motores conectados (sin carga):
2. Neutral → UP izquierdo
   - ✓ Motor izquierdo arranca suavemente
   - ✓ Velocidad aumenta gradualmente
   - ✓ Motor derecho permanece detenido
3. UP → Neutral
   - ✓ Motor izquierdo se detiene inmediatamente
4. Neutral → DOWN izquierdo
   - ✓ Motor izquierdo gira en sentido contrario
   - ✓ Velocidad aumenta gradualmente

## Ajuste de Parámetros

Si necesitas ajustar el comportamiento:

### Aceleración más rápida:
```cpp
const int ACELERACION = 10;  // Duplica la velocidad de aceleración
```

### Aceleración más suave:
```cpp
const int ACELERACION = 2;   // Aceleración más gradual
```

### Ciclo de actualización más frecuente:
```cpp
const int DELAY_LECTURA = 25; // Actualiza cada 25ms (más responsivo)
```

## Problemas Comunes

### El motor no arranca suavemente
- Aumentar `ACELERACION` para arranque más rápido
- Verificar que el motor no tenga demasiada carga

### No responde al joystick
- Verificar conexiones de pines
- Verificar que solo una señal esté activa a la vez
- Usar resistencias pull-down si el joystick no las incluye

### El motor no se detiene
- Verificar que la señal NEUTRAL llegue correctamente al Arduino
- Revisar el código de lectura de estados

### Velocidades incorrectas
- Verificar que `VELOCIDAD_MAXIMA = 255`
- Revisar valores en Monitor Serie para depuración
