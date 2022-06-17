#include <SoftwareSerial.h>
#include <IRremote.h>
int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);     
decode_results results; 

SoftwareSerial mySerial(8, 9); // RX, TX

byte speed = 0x64;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  irrecv.enableIRIn();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }


  //Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
}

void loop() // run over and over
{

  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);

if(results.value == 0xFB00629D)
{

byte check = speed^0x01^0x01;

mySerial.write(0xAA);
mySerial.write(0x03);
mySerial.write(0x01);
mySerial.write(0x01);
mySerial.write(speed);
mySerial.write(check);




}

  

}
