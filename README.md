# Microcontroller_Laboratory_Exercise - Block Stack Game on STM32 with FreeRTOS

This project implements a skill-based block-stacking game using the STM32 NUCLEO-F446RE microcontroller, a graphical LCD, and FreeRTOS. The game was developed as part of a microcontroller laboratory assignment at Budapest University of Technology and Economics.

## 🄹 Game Description

The player's goal is to stack moving blocks as precisely as possible. Any part of a block that overhangs the previous one will fall off. The game continues until no part of the new block overlaps the one below. A button is provided to restart the game quickly.

## 🔧 Hardware Overview

- **Microcontroller**: STM32 NUCLEO-F446RE  
- **Display**: Graphical LCD (with PWM-controlled backlight)  
- **Inputs**:  
  - Two buttons to control LCD backlight brightness  
  - One button to reset/restart the game  
- **Connections**:  
  - USB virtual serial port for PC communication  

Schematics and connection diagrams are included in the documentation.

## 🛏️ Firmware

The firmware is based on **FreeRTOS**, which handles real-time task scheduling efficiently. Key tasks:

- `CreateMyTasksInitTask`: Initializes the LCD and starts other tasks
- `GlcdUpdateTask`: Manages game logic, animation, and rendering
- `UartCommunicationTask`: Sends the final score to the PC
- `GlcdBacklightControlTask`: Adjusts the display brightness via PWM
- `GlcdBacklightPlusTask` / `MinusTask`: Increases or decreases backlight level
- `ResetTask`: Restarts the game upon button press

FreeRTOS made time-critical task coordination significantly more manageable than using a simple polling loop.

## 🖥️ PC Application

The accompanying PC client is built with a Windows Forms GUI. It performs the following:

- Displays incoming scores via USB virtual COM port
- Allows users to enter their nickname
- Saves and loads scoreboards to/from file
- Automatically ranks new scores on the leaderboard

## ♻️ Communication

The microcontroller communicates with the PC application over USB using a virtual UART interface. When the game ends, the final score is sent to the PC, which then adds it to the scoreboard under the player's name.

## 📸 Demo

Photos of the device in operation and full documentation are included in the repository.

## 💡 Lessons Learned

Using FreeRTOS proved to be a wise choice, simplifying task timing and system structure. Although buffered rendering helped simplify LCD updates, it was still challenging to display content perpendicular to the write direction.
