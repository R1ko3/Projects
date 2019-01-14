int sec;
int mnt;
int hou;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(D4, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
}

void loop() {
  sec++;
  if(sec >= 60){
    sec = 0;
    mnt++;
  }
  if(sec >= 60){
    mnt = 0;
    hou++;
  }
  digitalWrite(D4, LOW);
  shiftOut(D2, D1, LSBFIRST, sec);
  shiftOut(D2, D1, LSBFIRST, mnt);
  shiftOut(D2, D1, LSBFIRST, hou);
  digitalWrite(D4, HIGH);
  delay(1000);
}
