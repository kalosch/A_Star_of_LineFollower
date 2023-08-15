void motorA(int speed) {
  speed = speed * 2.55;
  if (speed >= 255) {
    speed = 255;
  } else if (speed <= -255) {
    speed = -255;
  }
  boolean inPin1;
  boolean inPin2;
  if (speed >= 0) {
    inPin1 = HIGH;
    inPin2 = LOW;
  } else if (speed < 0) {
    inPin1 = LOW;
    inPin2 = HIGH;
    speed = speed * -1;
  }
  digitalWrite(Motor_Right_Forward, inPin1);
  digitalWrite(Motor_Right_Reverse, inPin2);
  analogWrite(Motor_Right_Speed, speed);
}
void motorB(int speed) {
  speed = speed * 2.55;
  if (speed >= 255) {
    speed = 255;
  } else if (speed <= -255) {
    speed = -255;
  }
  boolean inPin1;
  boolean inPin2;
  if (speed >= 0) {
    inPin1 = HIGH;
    inPin2 = LOW;
  } else if (speed < 0) {
    inPin1 = LOW;
    inPin2 = HIGH;
    speed = speed * -1;
  }
  digitalWrite(Motor_Left_Forward, inPin1);
  digitalWrite(Motor_Left_Reverse, inPin2);
  analogWrite(Motor_Left_Speed, speed);
}
void Stop () {
  motorA(-100);
  motorB(-100);
  delay(50);
  motorA(0);
  motorB(0);
}
