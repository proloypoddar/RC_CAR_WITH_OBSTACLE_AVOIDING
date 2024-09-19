#include <Servo.h>

int ENA = 6;
int IN1 = 13;
int IN2 = 12;
int ENB = 5;
int IN3 = 11;
int IN4 = 10;
int speedValue = 0;

const int trigPin = 7;
const int echoPin = 8;

Servo servo;
const int servoPin = 9;

bool obstacleAvoidanceOn = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo.attach(servoPin);
  servo.write(90);
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();

    Serial.print("Received command: ");
    Serial.println(command);

    if (!obstacleAvoidanceOn) {
      if (command == 'F') {
        moveForward();
      } else if (command == 'B') {
        moveBackward();
      } else if (command == 'L') {
        turnLeft();
      } else if (command == 'R') {
        turnRight();
      } else if (command == 'S') {
        stopMovement();
      }
    }

    if (command >= '0' && command <= '9') {
      speedValue = map(command - '0', 0, 9, 0, 255);
      Serial.print("Speed set to: ");
      Serial.println(speedValue);
    }

    if (command == 'W') {
      obstacleAvoidanceOn = !obstacleAvoidanceOn;
      if (obstacleAvoidanceOn) {
        Serial.println("Obstacle avoidance ON");
      } else {
        Serial.println("Obstacle avoidance OFF");
      }
    }
  }

  if (obstacleAvoidanceOn) {
    performObstacleAvoidance();
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void stopMovement() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  
  return distance;
}

void performObstacleAvoidance() {
  long distance = measureDistance();

  if (distance < 29) {
    stopMovement();

    for (int angle = 0; angle <= 180; angle += 30) {
      servo.write(angle);
      delay(500);
      distance = measureDistance();
      
      if (distance > 20) {
        if (angle < 90) {
          turnLeft();
        } else {
          turnRight();
        }
        delay(1000);
        moveForward();
        break;
      }
    }
    servo.write(90);
  } else {
    moveForward();
  }
}