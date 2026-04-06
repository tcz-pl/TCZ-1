#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "24";
const char* password = "12345678";

WebServer server(80);

#define LED_PIN  2
int ledBrightness = 0;

void connectWiFi() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi: ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi连接成功！");
  Serial.print("网页地址: http://");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String html = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>ESP32无极调光器</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {text-align: center; margin-top: 60px; font-family: Arial;}
        .slider {width: 320px; height: 12px;}
        .value {font-size: 24px; color: #007bff; font-weight: bold;}
    </style>
</head>
<body>
    <h2>ESP32 LED无极调光</h2>
    <p>当前亮度值：<span id="brightVal" class="value">0</span></p>
    <input type="range" id="dimmer" min="0" max="255" value="0" class="slider">
    <script>
        const slider = document.getElementById('dimmer');
        const valText = document.getElementById('brightVal');
        slider.addEventListener('input', function() {
            const val = this.value;
            valText.textContent = val;
            fetch(`/set?bright=${val}`);
        });
    </script>
</body>
</html>
)HTML";
  server.send(200, "text/html", html);
}

void handleSetBright() {
  if (server.hasArg("bright")) {
    ledBrightness = server.arg("bright").toInt();
    ledBrightness = constrain(ledBrightness,0,255);
    analogWrite(LED_PIN, ledBrightness);
    server.send(200,"text/plain","OK");
  }else{
    server.send(400,"text/plain","ERROR");
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  connectWiFi();
  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
  Serial.println("服务器启动成功");
}

void loop() {
  server.handleClient();
}