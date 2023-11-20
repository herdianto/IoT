#define pinLED D1
#define pinButton D2

int buttonState;
int oldButton = 0;
int state = 0;
void setup() {
  Serial.begin(9600);
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
  if(!buttonState && !oldButton){
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