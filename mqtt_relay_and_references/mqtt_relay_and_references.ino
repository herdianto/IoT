#include <Preferences.h> //preference to store GPIO state
#include <ESP8266WiFi.h> //wifi connection
#include <PubSubClient.h> //mqtt connection

#define MSG_BUFFER_SIZE	(50)
#define relay1 D1
#define relay2 D4

Preferences pref;
WiFiClient espClient;
WiFiClient espClient_2;
PubSubClient client(espClient);
PubSubClient client_2(espClient_2);

const char* ssid = "HERDI_WIFI_Plus";
const char* password = "HerdiWifi123";
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_server_2 = "broker.hivemq.com";

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
int value = 0;
bool state_relay1 = LOW; //Define integer to remember the toggle state for relay 1
bool state_relay2 = LOW; //Define integer to remember the toggle state for relay 2

void setup() {
  Serial.begin(9600);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  setup_wifi();

  pref.begin("Relay_State", false);
  getRelayState(); // Get the last state of Relays

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback_2);
  client_2.setServer(mqtt_server_2, 1883);
  client_2.setCallback(callback);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client_2.connected()) {
    reconnect_2();
  }
  client.loop();
  client_2.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    value = 32; // should be from sensor
    //snprintf (msg, MSG_BUFFER_SIZE, "Temperature is :%ld", value);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish("herdi/device/temp", "mosquitto");
    //client_2.publish("herdi_2/device/temp", "hivemq");
  }
}

void getRelayState(){
  state_relay1 = pref.getBool("Relay1", 0);
  digitalWrite(relay1, state_relay1); 
  delay(200);
  state_relay2 = pref.getBool("Relay2", 0);
  digitalWrite(relay2, state_relay2); 
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

///*
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client_1-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("herdi/device/temp", "MQTT Server is Connected");
      // ... and resubscribe
      client.subscribe("herdi/device/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//*/

void reconnect_2() {
  // Loop until we're reconnected
  while (!client_2.connected()) {
    Serial.print("Attempting MQTT connection_2...");
    // Create a random client ID
    String clientId = "ESP8266Client_2-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client_2.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client_2.publish("herdi_2/device/temp", "MQTT Server is Connected");
      // ... and resubscribe
      client_2.subscribe("herdi_2/device/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client_2.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

///*
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
    digitalWrite(relay1, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    pref.putBool("Relay1", LOW);
  } else {
    digitalWrite(relay1, HIGH); 
    pref.putBool("Relay1", HIGH);
  }
}
//*/
void callback_2(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(relay2, HIGH);
    pref.putBool("Relay2", HIGH);
  } else {
    digitalWrite(relay2, LOW); 
    pref.putBool("Relay2", LOW);
  }
}