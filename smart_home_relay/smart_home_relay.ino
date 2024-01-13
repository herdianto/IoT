//#include <Preferences.h> //preference to store GPIO state
//#include <ESP8266WiFi.h> //wifi connection
//#include <PubSubClient.h> //mqtt connection
//#include <DHT.h> //temp and humidity library
#include <IRremote.h> //Infrared sensor library

//#define MSG_BUFFER_SIZE	(50)
//#define DTYPE DHT11 

//#define sw1 D1
#define RECEIVER_PIN 5
/*
#define sw2 D2
#define sw3 D3
#define sw4 D4
#define sw5 D5
#define sw6 D6
#define sw7 D7
#define sw8 D8
*/
//DHT dht(1,DTYPE); //TX Pin

IRrecv receiver(RECEIVER_PIN);
decode_results results;

//unsigned long lastMsg = 0;
//bool flag = 0;

void setup() {
  Serial.begin(9600);
  //dht.begin();
  //pinMode(sw1, OUTPUT);
  receiver.enableIRIn();
  /*
  pinMode(sw2, OUTPUT);
  pinMode(sw3, OUTPUT);
  pinMode(sw4, OUTPUT);
  pinMode(sw5, OUTPUT);
  pinMode(sw6, OUTPUT);
  pinMode(sw7, OUTPUT);
  pinMode(sw8, OUTPUT);
  pinMode(1, OUTPUT);
  */
  }

void loop() {
  //unsigned long now = millis();
  if(receiver.decode(&results)){
    Serial.println(results.value, HEX);
    receiver.resume();
  }/*
  if (now - lastMsg > 10000) {
    lastMsg = now;
    if (flag == 0){
      //Serial.println("switching all to low");
      //digitalWrite(sw1, LOW);
      digitalWrite(sw2, LOW);
      digitalWrite(sw3, LOW);
      digitalWrite(sw4, LOW);
      digitalWrite(sw5, LOW);
      digitalWrite(sw6, LOW);
      digitalWrite(sw7, LOW);
      digitalWrite(sw8, LOW);
      digitalWrite(1, LOW);
      flag = 1;
    }else{
      //Serial.println("switching all to high");
      //digitalWrite(sw1, HIGH);
      digitalWrite(sw2, HIGH);
      digitalWrite(sw3, HIGH);
      digitalWrite(sw4, HIGH);
      digitalWrite(sw5, HIGH);
      digitalWrite(sw6, HIGH);
      digitalWrite(sw7, HIGH);
      digitalWrite(sw8, HIGH);
      digitalWrite(1, HIGH);
      flag = 0;
    }
  }
  */
}
