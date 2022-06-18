/*
 Comentario 
 
*/
#include <ESP8266WiFi.h>

#include <WiFiClient.h>

#include <ESP8266WebServer.h>

#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "WIFI_EY_0"
#define STAPSK  "23852385"
#endif
#define ESP8266_GPIO4    4


const char* ssid     = STASSID;
const char* password = STAPSK;

IPAddress ip(192,168,0,200);     
IPAddress gateway(192,168,0,1);   
IPAddress subnet(255,255,255,0);   


ESP8266WebServer server(8080);

const int led = LED_BUILTIN;

const String postForms = "Hello World!";

unsigned long previousMillis = 0;
unsigned long interval = 43200000;

byte speed = 0x64;


void handleRoot() {
  
  server.send(200, "text/html", postForms);
  
}

void handleGetArguments(){
  String message = server.arg("value");

  
  if(message == "8"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x01^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
   Serial.write(0x01);
   Serial.write(0x01);
   Serial.write(speed);
   Serial.write(check);
   //Serial.println("fuck");

  
  }
  if(message == "5"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x00^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x00);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }
  
    if(message == "2"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x02^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x02);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }

    if(message == "4"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x03^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x03);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }
  if(message == "6"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x04^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x04);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }

  if(message == "7"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x05^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x05);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }

  if(message == "9"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x06^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x06);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }

  if(message == "3"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x07^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x07);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }

  if(message == "1"){
  server.send(200, "text/plain", message);
  //digitalWrite( ESP8266_GPIO4, HIGH );
  //Serial.println("encendido");
  //digitalWrite(led, 1);
  byte check = speed^0x08^0x01;

   Serial.write(0xAA);
  Serial.write(0x03);
Serial.write(0x08);
Serial.write(0x01);
Serial.write(speed);
Serial.write(check);
 
  }


  
  
  
  }

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode( ESP8266_GPIO4, OUTPUT );
  digitalWrite(led, 0);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
//  WiFi.setAutoReconnect(true);
//  WiFi.persistent(true);

  if (MDNS.begin("esp8266")) {
    //Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);


  server.on("/getform", handleGetArguments);

  server.begin();
  //Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  
}
