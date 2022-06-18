#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
int a=0;
char c;
char d;
byte speed = 0x64;
void setup() {
  Serial.begin(9600);
  //Serial.println("Goodnight moon!");
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
}
void loop() {
  delay(10);
  if (Serial.available()) {
    c=Serial.read();
    delay(10);
    byte check = speed^0x01^0x01;

    mySerial.write(0xAA);
    mySerial.write(0x03);
    mySerial.write(0x01);
    mySerial.write(0x01);
    mySerial.write(speed);
    mySerial.write(check);
  }
  delay(10);
  if (mySerial.available()) {
    d=mySerial.read();
    delay(10);
    
    Serial.write(d);

  }
}
