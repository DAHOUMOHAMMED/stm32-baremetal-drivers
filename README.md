# stm32-baremetal-drivers

Bare-metal peripheral drivers in C for the **STM32F446RE** (ARM Cortex-M4), written from the reference manual, **without the HAL**.

I follow the Udemy course *Mastering Microcontroller and Embedded Driver Development (MCU1)* by FastBit Embedded Brain Academy. The course targets the STM32F407 Discovery board. I work on a **NUCLEO-F446RE**, so I adapt addresses, register structures, pins and clock bits by reading the F446 documents myself.


## Hardware

- NUCLEO-F446RE (on-board ST-LINK for flashing and debugging)
- External LED + series resistor, push button (breadboard)
- Logic analyzer, 8 channels, 24 MHz, used with PulseView (to check SPI/I2C signals)

## Tools

- C (GCC ARM toolchain), STM32CubeIDE, ST-LINK debugger
- Git / GitHub
- Documents: RM0390 (F446 reference manual), UM1724 (Nucleo-64 board), PM0214 (Cortex-M4 programming manual)
- No HAL and no third-party library in the drivers

## Repository layout

```
.
├── 001HelloWorld/           first project
├── 002PeriClockEnable/      enabling a peripheral clock through RCC
├── HSI_Clock_Output/        clock output experiment
└── stm32f4xx_drivers/
    ├── drivers/
    │   ├── Inc/             stm32f446xx.h (MCU header), GPIO and SPI driver headers
    │   └── Src/             GPIO and SPI driver sources
    └── Src/                 exercises (one main() active at a time)
```

## What is implemented

**MCU header (`stm32f446xx.h`)**
- Base addresses (memory, buses, peripherals)
- Register structures: GPIO, RCC, SPI, EXTI
- Clock enable/disable and reset macros
- NVIC register addresses and IRQ numbers

**GPIO driver**
- Init / DeInit, peripheral clock control
- Read/write a pin or a port, toggle
- Input, push-pull / open-drain output, alternate function, pull-up / pull-down
- IRQ configuration, IRQ priority, IRQ handling (EXTI pending flag)

**SPI driver**
- Init / DeInit, peripheral clock control, peripheral enable
- Blocking `SPI_SendData` (8-bit and 16-bit frames)
- Flag status helper, SSOE configuration

**I2C driver**
- Init and master send, in progress

## Status

| Item | Status |
|---|---|
| GPIO output (LED toggle) | Tested on hardware |
| GPIO input (button) | Tested on hardware |
| GPIO interrupts (EXTI + NVIC) | Tested on hardware |
| SPI master TX | Tested on hardware |
| I2C master TX | Tested on hardware |
| SPI RX, I2C RX, USART | Tested on hardware |


## Exercises

| File | What it does |
|---|---|
| `001_led_toggle.c` | Toggles the on-board LED (PA5) |
| `003_*` | External LED (PA8) controlled by an external button (PB12) |
| `005_spi_tx_testing.c` | Sends a length byte, then "Hello World", on SPI2 when the user button (PC13) is pressed |
| `008i2c_master_tx_testing.c` | I2C1 master send |

Each exercise has its own `main()`. Only one can be built at a time: exclude the others from the build in CubeIDE.

## Adapting the course to the F446

Differences I had to handle compared with the course (F407 Discovery):

- Different reference manual (RM0390), more RCC registers, no GPIOI
- User LED on PA5 and button on PC13 instead of PD12-PD15 and PA0
- 64-pin package: not every port or pin exists on the board
- PA13 / PA14 are the debug pins, so they are not used as GPIO

## Build and run

1. Clone the repository and import the project into STM32CubeIDE (*File > Import > Existing Projects*).
2. Make sure `drivers/Inc` is in the include paths and `drivers/Src` is a source location.
3. Build, then flash with the ST-LINK.



## Author

Mohammed Dahou, electronics and embedded systems engineer.
