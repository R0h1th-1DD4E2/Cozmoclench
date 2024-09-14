import pygame
import asyncio
import websockets
import sys

# WebSocket connection settings
def get_ip():
    if len(sys.argv) > 1:
        # Use the IP passed as an argument
        return sys.argv[1]
    else:
        # Prompt the user to enter the IP
        return input("Enter the IP address of the bot (e.g., ws://192.168.1.47:8080): ")

URI = get_ip()

# Initialize pygame and joystick
pygame.init()
pygame.joystick.init()

# Connect to WebSocket server
async def send_command(command, speed1=0, speed2=0, angle=90):
    async with websockets.connect(URI) as websocket:
        message = f"{command} {speed1} {speed2} {angle}"
        print(f"Sending: {message}")
        await websocket.send(message)

def get_joystick_input():
    joystick = pygame.joystick.Joystick(0)
    joystick.init()

    angle = 90 

    while True:
        pygame.event.pump()
        
        # Left stick Y axis for forward/backward
        left_stick_y = joystick.get_axis(1)
        # Right stick X axis for turning
        right_stick_x = joystick.get_axis(3)
        # Triggers for speed control
        right_trigger = (joystick.get_axis(5) + 1) / 2 
        left_trigger = (joystick.get_axis(2) + 1) / 2  

        # Get button input for gripper control
        a_button = joystick.get_button(0)  # A button (increase angle)
        b_button = joystick.get_button(1)  # B button (decrease angle)

        command = "STP"
        speed1, speed2 = 0, 0

        if left_stick_y < -0.5:
            command = "FWD"
            speed1 = int(255 * right_trigger)
            speed2 = int(255 * right_trigger)
        elif left_stick_y > 0.5:
            command = "BWD"
            speed1 = int(255 * right_trigger)
            speed2 = int(255 * right_trigger)
        elif right_stick_x > 0.5:
            command = "RT"
            speed1 = int(255 * right_trigger)
            speed2 = int(255 * right_trigger)
        elif right_stick_x < -0.5:
            command = "LT"
            speed1 = int(255 * right_trigger)
            speed2 = int(255 * right_trigger)

        if a_button:
            angle += 5
        if b_button:
            angle -= 5

        if angle > 50:
            angle = 50
        if angle < 0:
            angle = 0

        # Send command over WebSocket
        asyncio.run(send_command(command, speed1, speed2, angle))

        pygame.time.wait(100)

if __name__ == "__main__":
    get_joystick_input()
