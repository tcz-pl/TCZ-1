#define TOUCH_PIN 4  
const int LED_A_PIN = 2; 
const int LED_B_PIN = 5;
const int freq = 5000;
const int resolution = 8;
int threshold = 300;
bool lastTouchState = false;
int speedGear = 1;
int step = 1;
void setup() {
  Serial.begin(115200);
  ledcAttach(LED_A_PIN, freq, resolution);
  ledcAttach(LED_B_PIN, freq, resolution);
  Serial.println("双LED反相呼吸灯 · 3档触摸调速");
  Serial.println("触摸切换：慢 → 中 → 快 → 慢");
}
void loop() {
  int touchValue = touchRead(TOUCH_PIN);
  bool currentTouch = (touchValue < threshold);
  if (currentTouch == true && lastTouchState == false) {
    speedGear++;
    if (speedGear > 3) speedGear = 1;
    switch (speedGear) {
      case 1: step = 1;  break; 
      case 2: step = 4;  break;
      case 3: step = 8;  break;
    }
    Serial.print("当前速度档位：");
    Serial.println(speedGear);
    delay(50);
  }
  lastTouchState = currentTouch;
  for (int duty = 0; duty <= 255; duty += step) {
    ledcWrite(LED_A_PIN, duty);
    ledcWrite(LED_B_PIN, 255 - duty);
    delay(6);
  }
  for (int duty = 255; duty >= 0; duty -= step) {
    ledcWrite(LED_A_PIN, duty);
    ledcWrite(LED_B_PIN, 255 - duty); 
    delay(6);
  }
}