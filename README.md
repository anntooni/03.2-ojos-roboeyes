# Semana 03.2 — RoboEyes OLED, control de motores y FSM de arranque

[![Ver Guía Maestra en Vivo](https://img.shields.io/badge/Guía_Técnica-Ver_en_Línea-E07A5F?style=for-the-badge&logo=cloudflare)](https://uets-st-portal.vgmiltonisaac.workers.dev/03.2-ojos-roboeyes/)

Sistema Embebido ESP32 · Soporte Técnico 3° BGU · Unidad Educativa Técnico Salesiano · 2026–2027

Este proyecto para ESP32 combina la animacion RoboEyes, una pantalla de arranque y el
control de dos motores reductores mediante un L293D. Los comandos se reciben por el
Monitor Serie y el OLED muestra el sentido de movimiento con flechas animadas.

---

## Inicio rápido en 4 pasos

1. **Abre el espacio de trabajo.** Abre `03.2-ojos-roboeyes.code-workspace` en VS Code.
   Sus cuatro raíces etiquetadas apuntan al proyecto plano, así el panel de PlatformIO
   encuentra `platformio.ini` y no aparece el error *"Nothing to build"*.
2. **Revisa el cableado y los pines** de las tablas de esta guía. Los motores necesitan
   alimentación propia a través del puente H; no se conectan directamente al ESP32.
3. **Compila desde el panel de PlatformIO.** Usa el icono de la hormiga 🐜 (**Build**) en la
   barra de PlatformIO. No necesitas terminal: la compilación debe quedar limpia, sin warnings.
4. **Simula en Wokwi.** Con `diagram.json` y `wokwi.toml` en la raíz, ejecuta la simulación y
   revisa la telemetría en el Monitor Serie a **115200 bps**.

> **Prueba en hardware físico:** el flasheo del ESP32 real se hace siempre en **VS Code
> local** (laboratorio o tu máquina). Un entorno de desarrollo hospedado no puede pasar
> puertos COM físicos hacia el microcontrolador.

---

## Motores, L293D y cableado

Materiales: placa ESP32, dos motores reductores, L293D en su porta-integrados, borneras
para sujetar el cableado, pantalla OLED SSD1306 y bateria de litio con modulo de carga y
proteccion compatibles.

El L293D es un puente H doble: cada mitad controla el sentido de un motor y su entrada
`EN` permite regular la potencia con PWM. Conecta `EN1`, `IN1`, `IN2` al motor izquierdo,
y `EN2`, `IN3`, `IN4` al motor derecho. Une todas las tierras (ESP32, L293D y batería).
Alimenta la lógica del L293D por `VCC1` y los motores por `VCC2`, respetando los voltajes
nominales de la placa, el integrado y los motores. No alimentes los motores desde un pin
del ESP32 ni conectes una batería de litio sin protección y cargador adecuados.

| Senal del L293D | ESP32 | Uso |
|---|---:|---|
| `EN1` | GPIO25 | PWM motor izquierdo |
| `IN1`, `IN2` | GPIO27, GPIO14 | Sentido motor izquierdo |
| `EN2` | GPIO26 | PWM motor derecho |
| `IN3`, `IN4` | GPIO32, GPIO33 | Sentido motor derecho |
| `GND` | GND comun | Referencia compartida |
| `VCC1` / `VCC2` | Fuente logica / bateria adecuada | Logica / motores |

El firmware genera PWM a 20 kHz y 8 bits: `ledcWrite` cambia el ciclo de trabajo entre
0 y 255. La frecuencia indica cuantas veces por segundo se repite el pulso; el ciclo de
trabajo indica que fraccion del periodo queda activo. Por ejemplo, 128/255 entrega cerca
del 50 % del tiempo activo y, idealmente, la mitad del voltaje medio disponible para el
motor antes de las perdidas del driver. Reducir el ciclo suele reducir la velocidad,
aunque la carga, el motor y el driver tambien influyen.

Si un motor zumba pero no gira, puede estar bloqueado o necesitar mas par para arrancar;
tambien revisa la bateria, el cableado, el puente H y que el ciclo de trabajo no sea muy
bajo. Detenlo si no gira: mantenerlo bloqueado puede sobrecalentar el motor o el driver.
El cargador debe corresponder al numero y tipo de celdas; no uses el robot mientras carga
salvo que el modulo soporte explicitamente una carga conectada.

### Comandos

Abre el Monitor Serie a 115200 bps y usa `W` adelante, `S` atras, `A` giro a la izquierda,
`D` giro a la derecha y `Espacio` o `0` para detener. Los giros son sobre el sitio: las
ruedas giran en sentidos opuestos. `+` y `-` cambian la velocidad; `1` a `7` muestran
expresiones de ojos y `H` imprime la ayuda. Para volver a los ojos, primero detén los
motores. El OLED muestra una flecha animada durante el movimiento y una señal de parada
cuando los motores estan detenidos.

`diagram.json` contiene solo la placa y el OLED para conservar la simulacion existente;
el control del L293D y los motores se verifica con el cableado fisico de arriba.

---

## Inventario del proyecto (plano, sin subcarpetas por reto)

| Archivo | Responsabilidad |
|---|---|
| `main.ino` | Orquesta el arranque y el bucle principal |
| `config.h` | Configuración global única (pines, dirección, geometría, tiempos) |
| `i2c_manager.h` | Bus I2C: pines, velocidad, escaneo y verificación |
| `display.h` | Pantalla OLED SSD1306: inicialización y texto |
| `logo.h` | Datos del bitmap del logo de arranque |
| `logboot.h` | Logo de arranque y POST de pantalla |
| `eyes.h` | Ojos animados (RoboEyes) y expresión elegida |
| `debug_serial.h` | Comandos del Monitor Serie y bloque de ayuda |
| `motor_pwm.h` | PWM, velocidad y dirección de los dos motores |
| `motor_display.h` | Flechas animadas y estado de movimiento en el OLED |
| `platformio.ini` | Entorno ESP32 DevKit con framework de Arduino |
| `diagram.json` | Esquema Wokwi (ESP32 + SSD1306 en `0x3C`) |
| `wokwi.toml` | Ruta del firmware para el simulador |
| `03.2-ojos-roboeyes.code-workspace` | Espacio de trabajo multiraíz |
| `CHEATSHEET_ESTUDIANTE.md` | Pines, direcciones, teclas y commits |

Regla de estructura: **cero archivos `.cpp`** y **cero diapositivas** en este repositorio.
Todo el código vive en el boceto `.ino` y en los encabezados de módulo.

---

## Entrega

1. Haz el **fork** del repositorio del docente y clona **tu** fork.
2. Crea tu rama de entrega: `git checkout -b entrega/nombre-apellido`.
3. Commits con **Conventional Commits** (`feat:`, `fix:`, `docs:`, `refactor:`, `chore:`).
4. Abre el Pull Request comparando `compare: entrega/nombre-apellido` contra `base: main`.

### Reglamento de tiempos del video

| Entrega | Límite | Penalización |
|---|---|---|
| Short de anticipación | 60 s (gracia hasta 65 s) | Más de 65 s → −0.50 pts · más de 90 s → se califica sobre el 50 % |
| Screencast demostrativo | 3 a 4 min sugeridos · tope absoluto **5:00** | Desde 5:01 → −1.00 pts |

El enlace público del screencast va en la descripción del Pull Request. Si terminas la clase
con avance parcial, entrega igual: la sustentación oral se rescata completa.
