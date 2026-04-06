#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "24";
const char* password = "12345678";

WebServer server(80);

#define LED_PIN 2
#define TOUCH_PIN T0

bool isArmed = false;
bool isAlarming = false;

void connectWiFi() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String html = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>安防报警器</title>
<style>
body{text-align:center;margin-top:50px;}
button{width:180px;height:60px;font-size:22px;margin:20px;border:none;border-radius:10px;}
.arm{background:#28a745;color:white;}
.disarm{background:#dc3545;color:white;}
</style>
</head>
<body>
<h2>ESP32 物联网安防报警器</h2>
<button class="arm" onclick="fetch('/arm')">布防(Arm)</button>
<button class="disarm" onclick="fetch('/disarm')">撤防(Disarm)</button>
</body>
</html>
)HTML";
  server.send(200, "text/html", html);
}

void handleArm() {
  isArmed = true;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "Armed");
}

void handleDisarm() {
  isArmed = false;
  isAlarming = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "Disarmed");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  connectWiFi();
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  
  if(isArmed && !isAlarming){
    if(touchRead(TOUCH_PIN) < 30){
      isAlarming = true;
    }
  }

  if(isAlarming){
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}