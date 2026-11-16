# CHEATSHEET — Semana 03.2: RoboEyes, motores y FSM de arranque

Sistema Embebido ESP32 · Soporte Técnico · 3° BGU · Unidad Educativa Técnico Salesiano

> Recordatorio Deep Freeze: configura tu identidad de Git al sentarte y trabaja siempre
> en tu rama `entrega/nombre-apellido`. Nunca entregues sobre `main`.

---

## 1. Referencia rápida de hardware

| Parámetro | Valor | Dónde vive |
|---|---|---|
| Pin SDA | `21` | `config.h` |
| Pin SCL | `22` | `config.h` |
| Velocidad del bus I2C | `400 kHz` | `config.h` |
| Dirección del panel OLED | `0x3C` | `config.h` |
| Geometría del panel | `128 x 64` | `config.h` |
| Pin de reset | `-1` (no cableado) | `config.h` |
| Velocidad del Monitor Serie | `115200 bps` | `main.ino` |
| Ventana del logo de arranque | `3000 ms` | `config.h` |
| Cuadros por segundo objetivo | `60 fps` | `config.h` |
| Frecuencia PWM motores | `20 kHz` | `config.h` |
| Resolucion PWM | `8 bits` (0-255) | `config.h` |
| PWM motor izquierdo | GPIO25 (`EN1`) | `config.h` |
| PWM motor derecho | GPIO26 (`EN2`) | `config.h` |

El OLED se simula en Wokwi y se conecta igual en la mesa. Wokwi conserva el circuito
OLED original; los motores L293D se prueban con el cableado fisico de esta guia.

| ESP32 | OLED SSD1306 |
|---|---|
| `GPIO21` | `SDA` |
| `GPIO22` | `SCL` |
| `3V3` | `VCC` |
| `GND` | `GND` |

L293D: conecta `IN1/IN2` a GPIO27/GPIO14 y `IN3/IN4` a GPIO32/GPIO33. `EN1` y `EN2`
reciben PWM por GPIO25 y GPIO26. Usa alimentacion adecuada para la logica y los motores,
una tierra comun y una bateria de litio con cargador y proteccion apropiados. Nunca
conectes los motores directamente a los GPIO.

---

## 2. Consola de depuración (Monitor Serie a 115200 bps)

| Tecla | Accion |
|---|---|
| `1` | `DEFAULT` |
| `2` | `HAPPY` |
| `3` | `ANGRY` |
| `4` | `TIRED` |
| `5` | `SLEEPY` |
| `6` | Alerta (`SCARY`) |
| `7` | `CURIOUS` |
| `h` o `H` | Repite el bloque de ayuda |
| `w` | Adelante |
| `s` | Atras |
| `a` | Giro a la izquierda |
| `d` | Giro a la derecha |
| `Espacio` o `0` | Detener ambos motores |
| `+` o `-` | Subir o bajar velocidad |

Reglas de la consola que debes respetar en tu código de Arduino:

- Se lee **una sola tecla por vuelta** del bucle, sin bloquear la animación.
- Una tecla válida produce **respuesta inmediata** por el monitor.
- Los caracteres de control (incluido el salto de línea del Enter) **se ignoran en silencio**.
- Para mostrar las expresiones de ojos, primero detén los motores.

### Ideas clave para explicar

- **PWM:** una senal que enciende y apaga rapidamente la salida. La frecuencia dice
  cuantas veces se repite por segundo; el ciclo de trabajo es el porcentaje de tiempo
  encendido y regula la potencia media que recibe el motor. Al 50 %, el voltaje medio
  ideal es aproximadamente la mitad del voltaje de alimentacion de los motores.
- **L293D:** puente H doble que permite cambiar el sentido de dos motores y recibir una
  senal PWM de habilitacion para ajustar su velocidad.
- **Motor que suena pero no gira:** puede estar atascado, no tener suficiente par de
  arranque o recibir poca tension; verifica cableado, bateria y driver y detenlo para
  evitar sobrecalentamiento.

---

## 3. Inventario de módulos (proyecto plano)

| Archivo | Responsabilidad |
|---|---|
| `main.ino` | Orquesta el arranque y el bucle; no dibuja nada |
| `config.h` | Configuración global única del proyecto |
| `i2c_manager.h` | Bus I2C: pines, velocidad, escaneo y verificación |
| `display.h` | Pantalla: inicialización y texto |
| `logo.h` | Datos del bitmap del logo de arranque |
| `logboot.h` | Logo de arranque y POST de pantalla |
| `eyes.h` | Ojos animados y expresión elegida |
| `debug_serial.h` | Comandos del Monitor Serie y ayuda |
| `motor_pwm.h` | PWM y funciones de movimiento |
| `motor_display.h` | Flechas y estado de movimiento en el OLED |

Regla de estructura: **cero archivos `.cpp`**. Todo el código vive en el boceto `.ino`
y en los encabezados de módulo, con un único `config.h`.

---

## 4. Conventional Commits

| Prefijo | Cuándo usarlo | Ejemplo |
|---|---|---|
| `feat:` | Nuevo reto resuelto o nueva función | `feat: resolver el escaneo del bus I2C` |
| `fix:` | Corrección de un error | `fix: centrar el cuadrado del POST` |
| `docs:` | Solo documentación | `docs: actualizar el cheatsheet` |
| `refactor:` | Reorganizar sin cambiar comportamiento | `refactor: extraer la ayuda de la consola` |
| `chore:` | Tareas de mantención | `chore: ignorar artefactos de compilación` |

---

## 5. Reglamento de tiempos del video

| Entrega | Límite | Penalización |
|---|---|---|
| Short de anticipación | 60 s (gracia hasta 65 s) | Más de 65 s → −0.50 pts · más de 90 s → se califica sobre el 50 % |
| Screencast demostrativo | 3 a 4 min sugeridos · **tope absoluto 5:00** | Desde 5:01 → −1.00 pts |

El docente califica estrictamente hasta el minuto 5:00. El enlace público del video va
en la descripción del Pull Request.

---

## 6. Secuencia de demostracion

1. Verifica que el OLED responda en `0x3C` y termina la ventana del logo.
2. Usa `W`, `S`, `A` y `D`; comprueba que las flechas del OLED coinciden con el giro.
3. Prueba `+` y `-` para explicar como el ciclo PWM ajusta la velocidad.
4. Pulsa espacio o `0`: deben apagarse los motores y el OLED debe mostrar detenido.
5. Usa `1` a `7` para regresar a las expresiones animadas de RoboEyes.

Cada paso depende del anterior: si el bus no responde, nada de lo demás se puede ver.
