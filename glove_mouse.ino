#include <Wire.h>
#include <MPU6050.h>
#include <Mouse.h>

MPU6050 mpu;
const int PIN_LEFT = 5;
const int PIN_RIGHT = 6;
const int PIN_MIDDLE = 7;
const int PIN_RECENTER = 4;

const int16_t DEADZONE = 300;

const float SCALE_X = 0.004;
const float SCALE_Y = 0.004;

const unsigned long CLICK_FREEZE_MS = 100;
unsigned long freezeUntil = 0;

long gyOffset = 0;
long gzOffset = 0;

float accX = 0.0;
float accY = 0.0;

bool prevLeft = HIGH;
bool prevRight = HIGH;
bool prevMiddle = HIGH;

bool mouseEnabled = true;

void calibrateGyro();
void handleButton();
void adjustMovement();

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(1000);

  // set up recenter pin early so the glove can potentially enter safe mode
  pinMode(PIN_RECENTER, INPUT_PULLUP);
  delay(10);
  if (digitalRead(PIN_RECENTER) == LOW) {
    mouseEnabled = false;
    Serial.println("SAFE MODE: Mouse disabled (recenter held at boot)");
  } else {
    mouseEnabled = true;
    Serial.println("Normal mode: Mouse will be enabled");
  }
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  calibrateGyro();
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_MIDDLE, INPUT_PULLUP);
  if (mouseEnabled) {
    Mouse.begin();
    Serial.println("Mouse started.");
  } else {
    Serial.println("Mouse NOT started (safe mode).");
  }
}

void loop() {
  unsigned long currTime = millis();

  bool leftState = digitalRead(PIN_LEFT);
  bool rightState = digitalRead(PIN_RIGHT);
  bool middleState = digitalRead(PIN_MIDDLE);
  bool recenterState = digitalRead(PIN_RECENTER);

  if (mouseEnabled) {
    handleButton(leftState, prevLeft, currTime, MOUSE_LEFT);
    handleButton(rightState, prevRight, currTime, MOUSE_RIGHT);
    handleButton(middleState, prevMiddle, currTime, MOUSE_MIDDLE);
  }

  prevLeft = leftState;
  prevRight = rightState;
  prevMiddle = middleState;

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  long rotationY = gy - gyOffset;
  long rotationZ = gz - gzOffset;
  if (abs(rotationY) < DEADZONE) {
    rotationY = 0;
  }
  if (abs(rotationZ) < DEADZONE) {
    rotationZ = 0;
  }

  // movement freeze occurs during recenter or after click
  bool freezeActive = false;
  if (recenterState == LOW || currTime < freezeUntil) {
    freezeActive = true;
  }
  if (freezeActive) {
    accX = 0.0;
    accY = 0.0;
    delay(10);
    return;
  }
  
  // normal movement
  accX += -(float)rotationY * SCALE_X;
  accY += -(float)rotationZ * SCALE_Y;
  int moveX = 0;
  int moveY = 0;
  adjustMovement(accX, moveX);
  adjustMovement(accY, moveY);
  if (mouseEnabled && (moveX != 0 || moveY != 0)) {
    Mouse.move(moveX, moveY, 0);
  }
  delay(10);
}

void calibrateGyro() {
  Serial.println("Calibrating gyroscope... KEEP YOUR HAND STILL!");
  const int samples = 1000;
  long gySum = 0;
  long gzSum = 0;
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  for (int i = 0; i < samples; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    gySum += gy;
    gzSum += gz;
    delay(2);
  }
  gyOffset = gySum / samples;
  gzOffset = gzSum / samples;
  Serial.print("Gyro offsets -> Y: ");
  Serial.print(gyOffset);
  Serial.print("  Z: ");
  Serial.println(gzOffset);
}

void handleButton(bool state, bool prev, unsigned long currTime, char button) {
  if (state == LOW && prev == HIGH) {
    Mouse.press(button);
    freezeUntil = currTime + CLICK_FREEZE_MS;
  }
  if (state == HIGH && prev == LOW) {
    Mouse.release(button);
  }
}

void adjustMovement(float &acc, int &move) {
  if (fabs(acc) >= 1.0f) {
    move = (int)acc;
    acc -= move;
  }
}
