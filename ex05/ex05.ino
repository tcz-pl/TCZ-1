#define TOUCH_PIN 4 
const int ledPin = 2; 
const int freq = 5000;
const int resolution = 8;
int threshold = 300;
int touchValue;
bool lastTouchState = false;
int speedGear = 1;
int step = 1;
void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
  Serial.println("三档调速呼吸灯已启动");
}
void loop() {
  touchValue = touchRead(TOUCH_PIN);
  bool currentTouch = (touchValue < threshold);
  if (currentTouch == true && lastTouchState == false) {
    speedGear++;
    if (speedGear > 3) {
      speedGear = 1;
    }
    switch (speedGear) {
      case 1: step = 1;  break;
      case 2: step = 4;  break;
      case 3: step = 8;  break;
    }
    Serial.print("当前档位：");
    Serial.println(speedGear);
    delay(50);
  }
  lastTouchState = currentTouch;
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle += step) {
    ledcWrite(ledPin, dutyCycle);
    delay(6);
  }
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle -= step) {
    ledcWrite(ledPin, dutyCycle);
    delay(6);
  }
}