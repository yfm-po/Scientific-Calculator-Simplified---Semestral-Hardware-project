# **Scientific Calculator Simplified - Semestral Hardware project**

## Introduction
This is a semestral team project at TUKE for the lesson Computer System Architectures. The calculator supports addition, subtraction, multiplication, and division, and also features a history function to display previous calculations. The main thing of that calculator is expression evaluation with any kind of brackets. The calculator is written in C and compiled to Arduino Uno. It uses a Keypad library to interface with the keypad, an LCD library to display the results, and a syntaxErrorHandler library to handle syntax errors in expressions. It also has an additional feature of a bomb defuse game which is triggered by pressing the "3" button when the CHANGE_MODE button is held down. The program implements history of the expressions and results, which can be viewed by pressing the "/" button while the CHANGE_MODE button is held down. The LEDs on the calculator are used to indicate the status of the calculator.

## Features
- Addition
- Subtraction
- Multiplication
- Division
- Expression evaluation with any kind of brackets
- Syntax error handling
- History
- Bomb defuse game

## Technologies Used
- C
- C++
- Arduino
- Keypad library
- LCD library

## Requirements
- Arduino Uno R3
- Keypad 4x4 AVR
- LCD 16x2 I2C Display
- Breadboard
- Passive buzzer
- 9V battery
- 9V battery jack
- Arduino button
- red LED diode
- blue LED diode
- resistor 220 Ohm
- resistor 1k Ohm
- Jumper cables

## Installation
To run the project on your local machine, follow these steps:
1. Connect the components to the Arduino Uno as shown in the circuit diagram
2. Clone the repository
3. Open the project in the Arduino IDE
4. Connect the Arduino Uno to your computer
5. Upload the project to the Arduino Uno
6. Enjoy the calculator and trolling your friends with the bomb defuse game

## Usage
To use the calculator, follow these steps:
1. Enter the expression you want to calculate
2. Press the "=" button to calculate the result
3. Press the "C" button to clear the display
4. Press the "CHANGE_MODE" button to enter the history mode
5. Press the "/" button to view the history
6. Press the "C" button to clear the history
7. Press the "CHANGE_MODE" button to exit the history mode
8. Press the "3" button while holding the "CHANGE_MODE" button to enter the bomb defuse game
9. Press the "C" button to exit the bomb defuse game

## Contributors
- [Renkoo](https://github.com/Renkooo)
- [Drascoll](https://github.com/SebastianOndrus)
- [Strepto](https://github.com/StreptoBordos)
- [xezrunner](https://github.com/xezrunner)
- [Khonsu](https://github.com/yfm-po)

## [Introductional video](https://www.youtube.com/watch?v=sny6hM6Z9BU&list=LL&index=7) by [xezrunner](https://github.com/xezrunner)
