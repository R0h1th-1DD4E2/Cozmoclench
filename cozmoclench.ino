#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "Your_SSID";
const char* password = "Your_Password";

#define in1u D0
#define in2u D1
#define enau D2 
#define in3u D3
#define in4u D5 
#define enbu D8

#define servo0 D4

const unsigned long commandTimeout = 1000;

unsigned long lastReceivedCommandTime = 0;

String lastReceivedCommand = "";

WebSocketsServer webSocket = WebSocketsServer(8080); 


int previous_angle = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  String message;
  char command[5];
  int speed1, speed2, angle;

  switch(type){
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);


      message = String((char*)payload);

      if (sscanf(message.c_str(), "%s %d %d %d", command, &speed1, &speed2, &angle) == 4) {
        move(command, speed1, speed2);
        
        if (previous_angle != angle) {
            writeServoAngle(angle);
            previous_angle = angle;
        }

        lastReceivedCommandTime = millis();
        lastReceivedCommand = String(command);
      }  
      else {
        Serial.println("Invalid message format");
      }

      break;
    default:
      break;
  }
}

void setupServo() {
    pinMode(servo0, OUTPUT);
    analogWriteRange(1023);
    analogWriteFreq(50);
}

void writeServoAngle(int angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    int pulseWidth = map(angle, 0, 180, 50, 190);

    analogWrite(servo0, pulseWidth);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 5) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi");
  }
}


void move(String command, int speed1, int speed2) {
  analogWrite(enau, speed1);
  analogWrite(enbu, speed2);
  
  if (command.equals("FWD")) {
    // Move forward
    digitalWrite(in1u, LOW);
    digitalWrite(in2u, HIGH);
    digitalWrite(in3u, LOW);
    digitalWrite(in4u, HIGH);
  } else if (command.equals("BWD")) {
    // Move backward
    digitalWrite(in1u, HIGH);
    digitalWrite(in2u, LOW);
    digitalWrite(in3u, HIGH);
    digitalWrite(in4u, LOW);
  } else if (command.equals("RT")) {
    // Turn right
    digitalWrite(in1u, LOW);
    digitalWrite(in2u, HIGH);
    digitalWrite(in3u, HIGH);
    digitalWrite(in4u, LOW);
  } else if (command.equals("LT")) {
    // Turn left
    digitalWrite(in1u, HIGH);
    digitalWrite(in2u, LOW);
    digitalWrite(in3u, LOW);
    digitalWrite(in4u, HIGH);
  } else if (command.equals("STP")) {
    // Stop
    digitalWrite(in1u, LOW);
    digitalWrite(in2u, LOW);
    digitalWrite(in3u, LOW);
    digitalWrite(in4u, LOW);
  }
  else {
    Serial.println("Unknown Command");
  }
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 8080");

  setupServo();

  Serial.println("WebSocket server started");

  pinMode(in1u, OUTPUT); 
  pinMode(in2u, OUTPUT); 
  pinMode(enau, OUTPUT);
  pinMode(in3u, OUTPUT);
  pinMode(in4u, OUTPUT);
  pinMode(enbu, OUTPUT);

}

void loop() {

  webSocket.loop();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    connectToWiFi();
  }

  if (millis() - lastReceivedCommandTime >= commandTimeout && lastReceivedCommandTime != 0 && lastReceivedCommand != "STP") {
    move("STP", 0, 0);
    lastReceivedCommand = "STP";
    Serial.println("STOP");
  }

  delay(10);
}