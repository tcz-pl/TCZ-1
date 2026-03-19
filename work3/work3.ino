const int ledPin = 16;

// 定义各阶段时间（单位：ms）
const unsigned long shortOn = 150;   // 短亮时长
const unsigned long shortOff = 150;  // 短灭时长
const unsigned long longOn = 450;    // 长亮时长
const unsigned long sosPause = 3000; // SOS周期结束后停顿

// 状态机枚举：定义SOS的各个阶段
enum State {
  S_SHORT1, S_SHORT2, S_SHORT3,
  L_LONG1, L_LONG2, L_LONG3,
  S_SHORT4, S_SHORT5, S_SHORT6,
  PAUSE
};

State currentState = S_SHORT1;
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentState) {
    // 3次短闪阶段
    case S_SHORT1:
    case S_SHORT2:
    case S_SHORT3:
    case S_SHORT4:
    case S_SHORT5:
    case S_SHORT6:
      if (ledState == LOW) {
        // 灭状态：到时间则亮灯
        if (currentMillis - previousMillis >= shortOff) {
          ledState = HIGH;
          digitalWrite(ledPin, HIGH);
          previousMillis = currentMillis;
        }
      } else {
        // 亮状态：到时间则灭灯，进入下一阶段
        if (currentMillis - previousMillis >= shortOn) {
          ledState = LOW;
          digitalWrite(ledPin, LOW);
          previousMillis = currentMillis;
          currentState = (State)(currentState + 1);
        }
      }
      break;

    // 3次长闪阶段
    case L_LONG1:
    case L_LONG2:
    case L_LONG3:
      if (ledState == LOW) {
        if (currentMillis - previousMillis >= shortOff) {
          ledState = HIGH;
          digitalWrite(ledPin, HIGH);
          previousMillis = currentMillis;
        }
      } else {
        if (currentMillis - previousMillis >= longOn) {
          ledState = LOW;
          digitalWrite(ledPin, LOW);
          previousMillis = currentMillis;
          currentState = (State)(currentState + 1);
        }
      }
      break;

    // 周期结束停顿阶段
    case PAUSE:
      if (currentMillis - previousMillis >= sosPause) {
        previousMillis = currentMillis;
        currentState = S_SHORT1; // 重置到SOS开头
      }
      break;
  }
}