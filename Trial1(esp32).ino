#include <WiFi.h> 

// WIFI HOTSPOT
const char* ssid = "ROBOTICARM";
const char* password = "12345678";

WiFiServer server(80);

// -------- IBT-2 Drivers (3 drivers = 6 motors) --------
#define RPWM1 26
#define LPWM1 25

#define RPWM2 32
#define LPWM2 33

#define RPWM3 19
#define LPWM3 18

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

void setup() {

  Serial.begin(115200);

  pinMode(RPWM1, OUTPUT); pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT); pinMode(LPWM2, OUTPUT);
  pinMode(RPWM3, OUTPUT); pinMode(LPWM3, OUTPUT);

  stopAll();

  // START WIFI HOTSPOT
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("WiFi Started");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

//------------------------------------------------------

void loop() {

  WiFiClient client = server.available();

  if (client) {

    String request = client.readStringUntil('\r');
    client.flush();

    Serial.println(request);

    // READ PWM
    if (request.indexOf("pwm=") != -1) {

      int start = request.indexOf("pwm=") + 4;
      int end = request.indexOf("&", start);

      if (end == -1)
        end = request.indexOf(" ", start);

      pwmValue = request.substring(start, end).toInt();
      pwmValue = constrain(pwmValue, 0, 1023);
    }

    // READ DIRECTION
    if (request.indexOf("dir=f") != -1)
      direction = 'f';

    if (request.indexOf("dir=b") != -1)
      direction = 'b';

    if (request.indexOf("dir=s") != -1)
      direction = 's';


    if (direction == 'f')
      moveForward();

    else if (direction == 'b')
      moveBackward();

    else
      stopAll();


    // WEBPAGE

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html\n");

    client.println("<html><body style='font-family:Arial;text-align:center;'>");

    client.println("<h2>ESP32 – 6 Wheel Control</h2>");

    client.println("<form action='/set'>");

    client.println("PWM (0–1023):<br>");

    client.print("<input type='number' name='pwm' value='");
    client.print(pwmValue);
    client.println("'><br><br>");

    client.println("<button name='dir' value='f'>Forward</button>");
    client.println("<button name='dir' value='b'>Backward</button>");
    client.println("<button name='dir' value='s'>Stop</button>");

    client.println("</form>");

    client.print("<p><b>PWM:</b> ");
    client.print(pwmValue);
    client.println("</p>");

    client.print("<p><b>Direction:</b> ");
    client.print(direction);
    client.println("</p>");

    client.println("</body></html>");

    client.stop();
  }
}
