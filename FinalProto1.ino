#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// ---------------- WIFI ----------------
ESP8266WebServer server(80);
WiFiManager wifiManager;

// ---------------- IBT-2 PINS ----------------
#define RPWM1 D1
#define LPWM1 D2
#define RPWM2 D3
#define LPWM2 D4
#define RPWM3 D5
#define LPWM3 D6

// ---------------- ESC ----------------
#define ESC_PIN D7
Servo esc;

const int ESC_MIN = 1000;
const int ESC_MAX = 2000;
const int ESC_START = 1150;
const int ESC_RUN_MAX = 1850;

// ---------------- VARIABLES ----------------
int pwmValue = 0;
char direction = 's';
int throttlePercent = 0;

// XPulse-like behavior variables
float rpm = 0;
float targetRPM = 0;
float speedKmh = 0;

const float idleRPM = 800;
const float maxRPM = 6000;

const float rpmResponse = 0.25;   // fast needle
const float speedResponse = 0.05; // slow speed rise
const float rpmToSpeedFactor = 0.02; // tuning factor

// --------------------------------------------------

void stopAll() {
  analogWrite(RPWM1, 0); analogWrite(LPWM1, 0);
  analogWrite(RPWM2, 0); analogWrite(LPWM2, 0);
  analogWrite(RPWM3, 0); analogWrite(LPWM3, 0);
}

void moveForward() {
  stopAll();
  analogWrite(RPWM1, pwmValue);
  analogWrite(RPWM2, pwmValue);
  analogWrite(RPWM3, pwmValue);
}

void moveBackward() {
  stopAll();
  analogWrite(LPWM1, pwmValue);
  analogWrite(LPWM2, pwmValue);
  analogWrite(LPWM3, pwmValue);
}

// --------------------------------------------------

void handleRoot() {

  String page = "<html><head>"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
  "<style>"
  "body{background:black;color:white;text-align:center;font-family:Arial;}"
  ".big{font-size:40px;margin:20px;}"
  "button{font-size:25px;padding:20px;margin:10px;width:200px;}"
  "</style></head><body>";

  page += "<h1>COCOCLIMB DASH</h1>";

  page += "<div class='big'>Speed: " + String(speedKmh,1) + " km/h</div>";
  page += "<div class='big'>RPM: " + String(rpm,0) + "</div>";

  page += "<form action='/set'>";

  page += "Throttle (0-100%)<br>";
  page += "<input type='number' name='thr' value='" + String(throttlePercent) + "'><br><br>";

  page += "<button name='dir' value='f'>FORWARD</button>";
  page += "<button name='dir' value='b'>REVERSE</button>";
  page += "<button name='dir' value='s'>STOP</button>";

  page += "</form></body></html>";

  server.send(200, "text/html", page);
}

// --------------------------------------------------

void handleSet() {

  if (server.hasArg("thr")) {
    throttlePercent = constrain(server.arg("thr").toInt(), 0, 100);
  }

  if (server.hasArg("dir")) {
    direction = server.arg("dir")[0];
  }

  pwmValue = map(throttlePercent, 0, 100, 0, 1023);

  if (direction == 'f') moveForward();
  else if (direction == 'b') moveBackward();
  else stopAll();

  server.sendHeader("Location", "/");
  server.send(303);
}

// --------------------------------------------------

void updateMotorBehavior() {

  // XPulse-style RPM behavior
  targetRPM = idleRPM + (throttlePercent / 100.0) * (maxRPM - idleRPM);

  rpm += (targetRPM - rpm) * rpmResponse;

  float linkedSpeed = rpm * rpmToSpeedFactor;
  speedKmh += (linkedSpeed - speedKmh) * speedResponse;

  // ESC Output
  int escSignal = ESC_START +
    (rpm / maxRPM) * (ESC_RUN_MAX - ESC_START);

  esc.writeMicroseconds(escSignal);
}

// --------------------------------------------------

void setup() {

  Serial.begin(115200);

  pinMode(RPWM1, OUTPUT); pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT); pinMode(LPWM2, OUTPUT);
  pinMode(RPWM3, OUTPUT); pinMode(LPWM3, OUTPUT);

  analogWriteRange(1023);
  stopAll();

  esc.attach(ESC_PIN);
  esc.writeMicroseconds(ESC_MIN);

  wifiManager.autoConnect("COCOCLIMB-SETUP");

  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
}

// --------------------------------------------------

void loop() {
  server.handleClient();
  updateMotorBehavior();
}
