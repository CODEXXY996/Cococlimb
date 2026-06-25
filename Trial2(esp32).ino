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
    if (request.indexOf("dir=f") != -1) direction = 'f';
    if (request.indexOf("dir=b") != -1) direction = 'b';
    if (request.indexOf("dir=s") != -1) direction = 's';

    if (direction == 'f') moveForward();
    else if (direction == 'b') moveBackward();
    else stopAll();

    int speedPercent = (pwmValue * 100) / 1023;

    // WEBPAGE

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    client.println("<html>");
    client.println("<head>");
    client.println("<meta charset='UTF-8'>");

    client.println("<style>");

    client.println("body{");
    client.println("background:radial-gradient(circle,#031a3a,#000814);");
    client.println("font-family:Orbitron,Arial;");
    client.println("color:#00eaff;");
    client.println("text-align:center;");
    client.println("margin:0;");
    client.println("}");

    client.println(".hud{");
    client.println("width:720px;");
    client.println("margin:auto;");
    client.println("margin-top:60px;");
    client.println("padding:40px;");
    client.println("border:2px solid #00eaff;");
    client.println("border-radius:6px;");
    client.println("box-shadow:0 0 35px #0077ff;");
    client.println("background:rgba(0,20,60,0.7);");
    client.println("}");

    client.println("h1{color:#00eaff;margin-bottom:30px;}");

    client.println("button{");
    client.println("width:180px;");
    client.println("padding:14px;");
    client.println("margin:10px;");
    client.println("font-size:15px;");
    client.println("border:none;");
    client.println("border-radius:4px;");
    client.println("cursor:pointer;");
    client.println("}");

    client.println(".forward{background:#00ff9c;color:black;}");
    client.println(".back{background:#ff3b3b;color:white;}");
    client.println(".stop{background:#ffaa00;color:black;}");

    client.println("input[type=range]{width:420px;}");

    client.println(".meter{");
    client.println("width:420px;");
    client.println("height:22px;");
    client.println("background:#02112a;");
    client.println("border:1px solid #00eaff;");
    client.println("margin:auto;");
    client.println("margin-top:15px;");
    client.println("}");

    client.println(".fill{");
    client.print("width:");
    client.print(speedPercent);
    client.println("%;");
    client.println("height:100%;");
    client.println("background:#00eaff;");
    client.println("}");

    client.println(".data{");
    client.println("margin-top:25px;");
    client.println("font-size:18px;");
    client.println("}");

    client.println("</style>");

    client.println("</head>");
    client.println("<body>");

    client.println("<div class='hud'>");

    client.println("<h1>6 WHEEL ROVER CONTROL PANEL</h1>");

    client.println("<form action='/set'>");

    client.println("<h3>THROTTLE CONTROL</h3>");

    client.print("<input type='range' min='0' max='1023' name='pwm' value='");
    client.print(pwmValue);
    client.println("' oninput='speedval.value=value'>");

    client.println("<br>");

    client.print("<output id='speedval'>");
    client.print(pwmValue);
    client.println("</output>");

    client.println("<br><br>");

    client.println("<button class='forward' name='dir' value='f'>FORWARD</button>");
    client.println("<button class='back' name='dir' value='b'>REVERSE</button>");
    client.println("<button class='stop' name='dir' value='s'>STOP</button>");

    client.println("</form>");

    client.println("<h3>SPEED INDICATOR</h3>");

    client.println("<div class='meter'><div class='fill'></div></div>");

    client.println("<div class='data'>");

    client.print("PWM OUTPUT : ");
    client.print(pwmValue);
    client.println("<br>");

    client.print("POWER LEVEL : ");
    client.print(speedPercent);
    client.println("%<br>");

    client.print("DIRECTION : ");
    client.print(direction);

    client.println("</div>");

    client.println("</div>");

    client.println("</body></html>");

    client.stop();
  }
}
