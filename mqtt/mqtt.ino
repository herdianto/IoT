#include <Preferences.h>
#define pinLED D1
#define pinButton D2

Preferences pref;

int buttonState;
// Relay State
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2

void getRelayState(){
  toggleState_1 = pref.getBool("Relay1", 0);
  digitalWrite(pinLED, toggleState_1); 
}

void setup() {
  Serial.begin(9600);
  pref.begin("Relay_State", false);
  pinMode(pinLED, OUTPUT);
  //pinMode(pinButton, INPUT);
  pinMode(pinButton, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(pinButton);
  //Serial.println(buttonState);
  if(!buttonState){
    digitalWrite(pinLED, LOW);
    Serial.println("LED OFF");
  }else{
    digitalWrite(pinLED, HIGH);
    Serial.println("LED ON");
  }
  /*
  if(!buttonState && !oldButton){e:\Download\Code_NodeMCU_Blynk2_Pref_4Relay_IR_DHT11\Code_NodeMCU_Blynk2_Pref_4Relay_IR_DHT11_Switch\Code_NodeMCU_Blynk2_Pref_4Relay_IR_DHT11_Switch.ino
    if(state == 0){
        digitalWrite(pinLED, HIGH);
        Serial.println("LED ON");
        state = 1;
      }else{
        digitalWrite(pinLED, LOW);
        Serial.println("LED OFF");
        state = 0;
      }
      oldButton = 1;
    }else if(buttonState && oldButton){
      oldButton = 0;
    }
  }
  */
}