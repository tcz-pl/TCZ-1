#define TOUCH_PIN 4
#define LED_PIN   2
int threshold = 300;
int touchValue;
bool ledState = false;
bool lastTouchState = false;
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}
void loop() {
  touchValue = touchRead(TOUCH_PIN);
  Serial.print("触摸值: ");
  Serial.println(touchValue);
  bool currentTouch = (touchValue < threshold);
  if (currentTouch == true && lastTouchState == false) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    delay(50);
  }
  lastTouchState = currentTouch;
}