# PIC18F2520 IR Remote-Controlled Mouse Emulator

This project implements an IR remote-controlled mouse emulator using the **PIC18F2520** microcontroller. It decodes signals from an NEC protocol IR remote and converts them into mouse movement and button click actions. These actions are output over UART as a simulated HID report for testing and potential integration with external USB HID interfaces.

---

## Overview

The goal of this project is to emulate basic USB mouse functionality using IR input, without relying on a native USB-enabled microcontroller. By using UART to simulate HID-like messages, it provides a foundation for integrating IR-based input with an external USB interface or for general debugging purposes.

---

## Hardware Requirements

| Component          | Details                                  |
|--------------------|-------------------------------------------|
| PIC18F2520         | 8-bit Microcontroller                     |
| MPLAB® PICkit™ 4   | Part number: PG164140                     |
| IR Receiver Module | Compatible with NEC protocol              |
| NEC Remote Control | Any remote using NEC protocol             |
| UART Interface     | USB-to-Serial module                      |
| Power Supply       | 5V regulated                              |
| Miscellaneous      | Breadboard, jumper wires, decoupling caps |

---

## System Diagram

```
 NEC Remote  →  IR Receiver  →  RB0/INT0
                                   ↓
                          PIC18F2520 (8MHz Internal + PLL)
                                   ↓
                            UART Output (RC6/TX)
                                   ↓
                         USB-to-Serial Interface
                                   ↓
                          Serial Terminal / Host MCU
```

---

## How It Works

### IR Decoding

- External interrupts on RB0 (INT0) are used to detect transitions in the NEC signal.
- Timer1 measures the duration of pulses and spaces.
- A state machine interprets the signal sequence:
  - 9ms leader pulse
  - 4.5ms space
  - 32-bit command
- Timer3 is used to detect repeated button presses.

### Mouse Emulation

Each decoded NEC code maps to a simulated mouse event, such as cursor movement or button clicks. These are packaged into a 4-byte array and transmitted over UART.

---

## IR Command Mapping

| NEC Code (Hex) | Action             |
|----------------|--------------------|
| 0x40BF30CF     | Left click         |
| 0x40BF708F     | Right click        |
| 0x40BFB04F     | Cursor up          |
| 0x40BF08F7     | Cursor left        |
| 0x40BF8877     | Cursor down        |
| 0x40BF48B7     | Cursor right       |

---

## UART Output Format

Each action is printed to the serial terminal using the following format:

```
Mouse: B=<button>, X=<x movement>, Y=<y movement>
```

Where:
- `B` is the button state:
  - `0` = No button pressed
  - `1` = Left-click
  - `3` = Right-click
- `X` is the horizontal cursor movement:
  - `-1` = Left
  - `0` = No movement
  - `1` = Right
- `Y` is the vertical cursor movement:
  - `-1` = Up
  - `0` = No movement
  - `1` = Down

### Example Output

```
Mouse: B=1, X=0, Y=0    // Left click
Mouse: B=0, X=1, Y=0    // Cursor right
Mouse: B=0, X=0, Y=-1   // Cursor up
Mouse: B=3, X=0, Y=0    // Right click
```

---

## Setup Instructions

1. **Compile the Code:**
   - Use MPLAB X IDE with the XC8 compiler.
   - Ensure the device is set to PIC18F2520.

2. **Flash the MCU:**
   - Use a compatible programmer (e.g., PICkit 3/4).

3. **Connect the Hardware:**
   - IR receiver output → RB0
   - TX (RC6) → USB-to-Serial RX
   - Power the board with 5V

4. **View Output:**
   - Open a serial terminal (TeraTerm, PuTTY, etc.)
   - Baud rate: **9600**
   - Observe output when pressing remote buttons

---
