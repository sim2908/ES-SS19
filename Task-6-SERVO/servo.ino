

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

  int angle = map(f, 0, 1023, -3, 3);
  return angle;
}

void moveServo(int posB, int posT) {
  servoB.write(posB);
  servoT.write(posT);
}

void savePos() {
  if (pos_pointer < 10) {
    positions[0][pos_pointer] = servoBAngle;
    positions[1][pos_pointer] = servoTAngle;
    pos_pointer++;
  }
  reportPos();
}

void deletePos() {
  pos_pointer = 0;
  reportPos();
  writeString(5, 150, "Cleared          ");
  writeBuffer();
}

void moveToPosition(int i) {
  if (i < 0 || i > ((pos_pointer - 1) * 50 )) {
    while (1);
  }

  if (i % 50 == 0) {
    moveServo(positions[0][i / 50], positions[1][i / 50]);
    return;
  }

  int b0 = positions[0][i / 50];
  int b1 = positions[0][(i / 50) + 1];

  int br = (int) (b0 + (((float) b1 - b0) * ((float) (i % 50) / 50)));

  int t0 = positions[1][i / 50];
  int t1 = positions[1][(i / 50) + 1];

  int tr = (int) (t0 + (((float) t1 - t0) * ((float) (i % 50) / 50)));
  
  moveServo(br, tr);
}

void playSequence() {
  if (pos_pointer <= 0) {
    return;
  }

  writeString(5, 150, "Now Playing...");
  writeBuffer();

  playFlag = true;
}

void reportPos() {
  initBuffer();
  writeString(5, 8, String(pos_pointer) + " / 10");
  for (int i = 0; i < pos_pointer; i++) {
    writeString(5, (i + 3) * 8, String(positions[0][i]) + "|" + String(positions[1][i]));
  }
  writeString(5, 150, "Recording...");
  writeBuffer();

}
