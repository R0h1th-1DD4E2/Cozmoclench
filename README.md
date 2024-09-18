# Gripper Bot for IIT Bombay Techfest [COZMO CLENCH]

This repository contains an example code and setup instructions for controlling a gripper bot via a WebSocket server using an ESP8266 module. The bot can be controlled using a web interface or game controller and is designed to drive through an obstacle-filled path and place objects in respective target zones, as per the competition requirements.

## Introduction

The **IIT Bombay Techfest** competition provides a platform for students to design and construct their own **gripper bot**, which can navigate through an obstacle-filled path and place objects in designated target zones. This competition encourages students to work in teams, apply engineering skills, and test their technical knowledge.

As part of this initiative, we are conducting an **introductory workshop** aimed at helping students understand the process of building a gripper bot. In this workshop, we will walk you through:
- Setting up a WebSocket-controlled gripper bot.
- Implementing motor control for movement (forward, backward, left, right).
- Controlling a servo for the gripper mechanism.
- Connecting the system via Wi-Fi for remote control using a browser or game controller.

## Components
- **ESP8266 Wi-Fi Module**
- **L298N Motor Driver** for controlling the motors
- **Servo Motor** for controlling the gripper
- **DC Motors** for driving the bot
- **Game Controller or Web Interface** for remote control

## Workshop Objectives
In the workshop, participants will learn:
1. Basic principles of motor control and servo motor mechanisms.
2. Setting up WebSocket communication for real-time control.
3. Writing the necessary embedded code to control the bot using ESP8266.
4. Implementing movement commands via a web interface and game controller.

## How to Use the Repository

### Prerequisites
- ESP8266 board (NodeMCU or similar)
- Motor driver (e.g., L298N)
- Servo motor
- $ DC motors for driving
- Wi-Fi network to connect to

### Step 1: ESP8266 Code Setup
1. Clone this repository:
   ```bash
   git clone https://github.com/R0h1th-1DD4E2/Cozmoclench.git
   cd Cozmoclench
   ```

2. Flash the `ESP8266` with the code in the `root` folder.

3. Modify the `ssid` and `password` in the code to connect the bot to your Wi-Fi network:
   ```cpp
   const char* ssid = "Your_SSID";
   const char* password = "Your_Password";
   ```

4. Upload the code using the Arduino IDE or another preferred method.

### Step 2: Web Interface Setup
You can control the bot using a simple web-based interface. The HTML file is located in the `controller` folder. Host this file on a local server or open it in your browser and paste the `ip address` of the bot.

### Step 3: Game Controller Support
To control the bot using a game controller, use the Python script located in the `controller` folder and do the same.

Install the necessary dependencies:
```bash
pip install pygame websockets
```

Run the script:
```bash
python controller.py
```

### Commands Overview
The bot understands several commands sent via WebSocket:
- **FWD**: Move forward
- **BWD**: Move backward
- **LT**: Turn left
- **RT**: Turn right
- **STP**: Stop
- **Angle Control**: Adjust the angle of the gripper's servo

