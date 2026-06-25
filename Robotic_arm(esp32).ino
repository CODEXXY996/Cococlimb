#include <WiFi.h>
#include <ESP32Servo.h>

// WIFI HOTSPOT
const char* ssid = "ROBOTICARM";
const char* password = "12345678";

Servo baseServo;
Servo link1Servo;
Servo link2Servo;
Servo bldcESC;

WiFiServer server(80);

bool escArmed = false;

// ESC ARM
void armESC() {
  Serial.println("Arming ESC...");

  bldcESC.writeMicroseconds(2000);
  delay(2000);

  bldcESC.writeMicroseconds(1000);
  delay(3000);

  escArmed = true;
  Serial.println("ESC ARMED");
}

void setup() {
  Serial.begin(115200);

  // YOUR PINS
  baseServo.attach(13);
  link1Servo.attach(12);
  link2Servo.attach(14);
  bldcESC.attach(27, 1000, 2000);

  // SAFE START
  baseServo.write(90);
  link1Servo.write(90);
  link2Servo.write(90);
  bldcESC.writeMicroseconds(1000);

  // START WIFI
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("WiFi Started");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    Serial.println(request);

    // REQUESTS
    if (request.indexOf("/arm") != -1) {
      armESC();
    }

    if (request.indexOf("/base?val=") != -1) {
      int val = request.substring(request.indexOf("=")+1).toInt();
      baseServo.write(val);   // ✅ FIXED
    }

    if (request.indexOf("/link1?val=") != -1) {
      int val = request.substring(request.indexOf("=")+1).toInt();
      link1Servo.write(val);
    }

    if (request.indexOf("/link2?val=") != -1) {
      int val = request.substring(request.indexOf("=")+1).toInt();
      link2Servo.write(val);
    }

    if (request.indexOf("/bldc?val=") != -1 && escArmed) {
      int val = request.substring(request.indexOf("=")+1).toInt();
      bldcESC.writeMicroseconds(val);
    }

    // WEBPAGE
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html\n");

    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");

    client.println("<style>");
    client.println("body { font-family: Arial; text-align: center; background: #f4f4f4; }");
    client.println("button { padding: 12px; background: red; color: white; border: none; border-radius: 8px; }");
    client.println("input[type=range] { width: 80%; }");
    client.println(".box { background: white; padding: 20px; margin: 15px; border-radius: 10px; }");
    client.println("</style>");

    client.println("</head><body>");

    client.println("<h2>Robotic Arm Control</h2>");

    // ARM
    client.println("<div class='box'>");
    client.println("<button onclick=\"fetch('/arm')\">ARM ESC</button>");

    if (escArmed)
      client.println("<p style='color:green;'>ESC ARMED</p>");
    else
      client.println("<p style='color:red;'>ESC NOT ARMED</p>");

    client.println("</div>");

    // BASE
    client.println("<div class='box'>");
    client.println("<h3>Base (0° - 180°)</h3>");
    client.println("<input type='range' min='0' max='180' value='90' oninput=\"fetch('/base?val='+this.value)\">");
    client.println("</div>");

    // LINK1
    client.println("<div class='box'>");
    client.println("<h3>Link 1</h3>");
    client.println("<input type='range' min='0' max='180' value='90' oninput=\"fetch('/link1?val='+this.value)\">");
    client.println("</div>");

    // LINK2
    client.println("<div class='box'>");
    client.println("<h3>Link 2</h3>");
    client.println("<input type='range' min='0' max='180' value='90' oninput=\"fetch('/link2?val='+this.value)\">");
    client.println("</div>");

    // BLDC
    client.println("<div class='box'>");
    client.println("<h3>BLDC Motor</h3>");

    if (escArmed)
      client.println("<input type='range' min='1000' max='2000' value='1000' oninput=\"fetch('/bldc?val='+this.value)\">");
    else
      client.println("<p>Arm ESC first</p>");

    client.println("</div>");

    client.println("</body></html>");

    client.stop();
  }
}