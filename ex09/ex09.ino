#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "24";
const char* password = "12345678";

WebServer server(80);
#define TOUCH_PIN T0

void connectWiFi() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String html = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>实时传感器仪表盘</title>
<style>
body{text-align:center;margin-top:100px;font-family:Arial;background:#f0f0f0;}
.value{font-size:80px;font-weight:bold;color:#2196F3;}
.label{font-size:24px;color:#666;}
</style>
</head>
<body>
<h2>ESP32 触摸传感器实时数据</h2>
<p class="label">触摸模拟量值：</p>
<h1 class="value" id="data">0</h1>
<script>
function updateData(){
  fetch('/data').then(res=>res.text()).then(val=>{
    document.getElementById('data').innerText=val;
  });
}
setInterval(updateData,50);
</script>
</body>
</html>
)HTML";
  server.send(200, "text/html", html);
}

void handleData() {
  int val = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(val));
}

void setup() {
  connectWiFi();
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}