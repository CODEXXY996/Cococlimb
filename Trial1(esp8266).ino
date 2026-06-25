#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WIFI HOTSPOT
const char* ssid = "WheelControl";
const char* password = "12345678";

ESP8266WebServer server(80);

// SET CUSTOM IP
IPAddress local_ip(192,168,29,115);
IPAddress gateway(192,168,29,115);
IPAddress subnet(255,255,255,0);



// -------- IBT-2 Drivers (3 drivers = 6 motors) --------
#define RPWM1 D1
#define LPWM1 D2

#define RPWM2 D3
#define LPWM2 D4

#define RPWM3 D5
#define LPWM3 D6

int pwmValue = 0;
char direction = 's';

//------------------------------------------------------

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

//------------------------------------------------------

void handleRoot() {

  String page =
  "<html><body style='font-family:Arial;text-align:center;'>"
  "<h2>ESP8266 – 6 Wheel Control</h2>"

  "<form action='/set'>"
  "PWM (0–1023):<br>"
  "<input type='number' name='pwm' value='" + String(pwmValue) + "'><br><br>"

  "<button name='dir' value='f'>Forward</button>"
  "<button name='dir' value='b'>Backward</button>"
  "<button name='dir' value='s'>Stop</button>"
  "</form>"

  "<p><b>PWM:</b> " + String(pwmValue) + "</p>"
  "<p><b>Direction:</b> " + String(direction) + "</p>"

  "</body></html>";

  server.send(200,"text/html",page);
}

//------------------------------------------------------

void handleSet() {

  if(server.hasArg("pwm"))
    pwmValue = constrain(server.arg("pwm").toInt(),0,1023);

  if(server.hasArg("dir"))
    direction = server.arg("dir")[0];

  if(direction=='f') moveForward();
  else if(direction=='b') moveBackward();
  else stopAll();

  server.sendHeader("Location","/");
  server.send(303);
}

//------------------------------------------------------

void setup() {

  Serial.begin(115200);

  pinMode(RPWM1,OUTPUT); pinMode(LPWM1,OUTPUT);
  pinMode(RPWM2,OUTPUT); pinMode(LPWM2,OUTPUT);
  pinMode(RPWM3,OUTPUT); pinMode(LPWM3,OUTPUT);

  analogWriteRange(1023);
  stopAll();

  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(local_ip,gateway,subnet);
  WiFi.softAP(ssid,password);

  Serial.println("WiFi Started");
  Serial.println(WiFi.softAPIP());

  server.on("/",handleRoot);
  server.on("/set",handleSet);

  server.begin();

  Serial.println("Web Server Started");
}

//------------------------------------------------------

void loop() {
  server.handleClient();
}
