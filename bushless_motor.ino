#include <Servo.h>

#define ESC_PIN D1

Servo esc;

int currentSpeed = 1000;
int targetSpeed  = 1000;

const int ESC_MIN   = 1000;
const int ESC_START = 1150;   // Adjust if needed
const int ESC_MAX   = 1850;

String inputString = "";
bool inputComplete = false;

void setup() {

  Serial.begin(115200);
  esc.attach(ESC_PIN);

  esc.writeMicroseconds(ESC_MIN);
  delay(3000);

  Serial.println("Enter throttle 0-100 and press Enter:");
}

void loop() {

  // Read serial properly
  while (Serial.available()) {

    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      inputComplete = true;
    } else {
      inputString += inChar;
    }
  }

  if (inputComplete) {

    int percent = inputString.toInt();
    percent = constrain(percent, 0, 100);

    if (percent == 0) {
      targetSpeed = ESC_MIN;
      Serial.println("Motor STOP");
    } else {
      float curve = pow(percent / 100.0, 2.0);  // smoother control
      targetSpeed = ESC_START + curve * (ESC_MAX - ESC_START);

      Serial.print("Throttle set to: ");
      Serial.print(percent);
      Serial.println("%");
    }

    inputString = "";
    inputComplete = false;
  }

  // Smooth ramp
  if (currentSpeed < targetSpeed) {
    currentSpeed += 2;
    esc.writeMicroseconds(currentSpeed);
    delay(4);
  }
  else if (currentSpeed > targetSpeed) {
    currentSpeed -= 2;
    esc.writeMicroseconds(currentSpeed);
    delay(4);
  }
}
