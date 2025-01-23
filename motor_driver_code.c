모터 드라이버 코드 (컨베이어벨트)

const int in1Pin = 8;      // L298N 모터 드라이버의 IN1 핀
const int in2Pin = 9;      // L298N 모터 드라이버의 IN2 핀
const int enablePin = 5;   // L298N 모터 드라이버의 ENA 핀 (PWM 제어 핀)


// 모터 속도 설정
int motorSpeed = 140;  // 모터 속도 (0 ~ 255)
int moveTime = 250;    // 모터가 10만큼 이동하는 시간 (밀리초)
int delayTime = 2000;  // 각 동작 사이의 대기 시간 (밀리초)

int state = 0;

void setup() {
  Serial.begin(9600);

 pinMode(enablePin, OUTPUT);
 pinMode(in1Pin, OUTPUT);
 pinMode(in2Pin, OUTPUT);
}

void moveLeft() {
  digitalWrite(in1Pin, HIGH);  // IN1 핀 HIGH (왼쪽 회전)
  digitalWrite(in2Pin, LOW);   // IN2 핀 LOW
  analogWrite(enablePin, motorSpeed);  // 속도 설정
  delay(moveTime);  // 10만큼 이동할 시간
}

void moveRight() {
  digitalWrite(in1Pin, LOW);   // IN1 핀 LOW
  digitalWrite(in2Pin, HIGH);  // IN2 핀 HIGH (오른쪽 회전)
  analogWrite(enablePin, motorSpeed);  // 속도 설정
  delay(moveTime);  // 10만큼 이동할 시간
}

void stopMotor() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enablePin, 0);   // 모터 정지
  delay(delayTime);  // 정지 후 대기 시간
}

void loop() {
  String received = "";

  switch (state) {
    case 0:
        stopMotor(); // case 3: 모터 정지
        break;

    case 1:
        moveLeft();  // case 1: 왼쪽 이동
        stopMotor(); // case 3: 모터 정지
        moveRight(); // case 2: 오른쪽 이동
        stopMotor(); // case 3: 모터 정지
   state = 0;
        break;

    case 2:
        moveRight(); // case 2: 오른쪽 이동
        stopMotor(); // case 3: 모터 정지
        moveLeft();  // case 1: 왼쪽 이동
        stopMotor(); // case 3: 모터 정지
   state = 0;
        break;

    default:
        stopMotor(); // 기본값으로 모터 정지
        break;
  }

  if (Serial.available() > 0) {
    received = Serial.readStringUntil('\n');
    Serial.println(received);
    if (received.indexOf("q") >= 0) {
      state = 2;
    }
    else if (received.indexOf("e") >= 0) {
      state = 1;
    }
    delay(100);
  }
  delay(100);
} 