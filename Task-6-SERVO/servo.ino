
int pos_pointer = 0;
int positions[2][10] = {{ }};


void directJoyToServo(int pin, Servo servo) {
  float f = (float) analogRead(pin);
  int angle = map(f, 0, 1023, 0, 180);
  servo.write(angle);
}

int relativeJoyToServo(int pin) {
  float f = (float) analogRead(pin);

  if (!(f > 562 || f < 462)) {
    return 0;
  }

  int angle = map(f, 0, 1023, -5, 5);
  return angle;
}

void moveServo(int posB, int posT) {

  int angleB = map(posB, 0, 1023, 0, 180);
  servoB.write(angleB);

  int angleT = map(posT, 0, 1023, 0, 180);
  servoT.write(angleT);

}

void savePos() {
  if (pos_pointer <= 10) {
    positions[0][pos_pointer] = servoBAngle;
    positions[1][pos_pointer] = servoTAngle;
    pos_pointer++;
    } 
  }

void deletePos() {
  pos_pointer = 0;
}

void moveToPosition(int i) {
  if (i < 0 || i >= ((pos_pointer - 1) * 10 )){
    while(1);
  }

  if (i % 10 == 0)
  {
    moveServo(positions[0][i / 10], positions[1][i / 10]);
    return;
  }

  int b0 = positions[0][i / 10];
  int b1 = positions[0][(i / 10) + 1];

  int br = b0 + ((b1 - b0) * ( i % 10 / 10));

  int t0 = positions[1][i / 10];
  int t1 = positions[1][(i / 10) + 1];

  int tr = t0 + ((t1 - t0) * ( i % 10 / 10));

  moveServo(br, tr);


}