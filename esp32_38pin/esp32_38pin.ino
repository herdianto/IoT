#include <WiFi.h> //wifi connection
#include <PubSubClient.h> //mqtt connection

#define io0 0
#define io1 1
#define io2 2
#define io4 4
#define io5 5
#define io12 12
#define io13 13
#define io14 14
#define io15 15
#define io16 16
#define io17 17
#define io18 18
#define io19 19
#define io21 21
#define io22 22
#define io23 23
#define io25 25
#define io26 26
#define io27 27
#define io32 32
#define io33 33

#define MSG_BUFFER_SIZE	(50)

WiFiServer server(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char* ssid = "HERDI_WIFI_Plus";
const char* password = "HerdiWifi123";
const char* ssidAP = "HERDI_ESP32";
const char* passwordAP = "HERDI_ESP32!!!";
// Set your Static IP address
IPAddress local_IP(192,168,18,201);
// Set your Gateway IP address
IPAddress gateway(192,168,18,1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192,168,18,1);   //optional
IPAddress secondaryDNS(8, 8, 8, 8); //optional

const char* mqtt_server = "test.mosquitto.org";

String header;
int count = 0;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 1000;
//mqtt timer
unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

void writeHTML(int gpioNo, WiFiClient client, String header){
  String label;
  String pinNo;
  pinNo = String(gpioNo);
  String urlON = "GET /"+pinNo+"/on"; 
  String urlOFF = "GET /"+pinNo+"/off";

  if (header.indexOf(urlON) >= 0) {
    Serial.println("GPIO "+pinNo+" on");
    digitalWrite(gpioNo, HIGH);
  } else if (header.indexOf(urlOFF) >= 0) {
    Serial.println("GPIO "+pinNo+" off");
    digitalWrite(gpioNo, LOW);
  }

  int pinStatus = digitalRead(gpioNo);
  if (pinStatus==LOW){
    label = "off";
  }else{
    label = "on";
  }
  client.println("<p>Device on GPIO "+pinNo+" is " + label + "</p>");
  if (pinStatus==LOW) {
    client.println("<p><a href=\"/"+pinNo+"/on\"><button class=\"button button2\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/"+pinNo+"/off\"><button class=\"button\">OFF</button></a></p>");
  } 
}

void setup() {
  Serial.begin(9600);
  // delete old config
  WiFi.disconnect(true);
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  delay(1000);
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.begin(ssid, password);
    
  Serial.println();
  Serial.println();
  Serial.println("Wait for WiFi... ");

  //WiFi.softAP(ssidAP, passwordAP);
  //web server
  server.begin();
  //mqtt server
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
  // Set pin mode
  pinMode(io0,OUTPUT);
  pinMode(io1,OUTPUT);
  pinMode(io2,OUTPUT);
  pinMode(io4,OUTPUT);
  pinMode(io5,OUTPUT);
  pinMode(io12,OUTPUT);
  pinMode(io13,OUTPUT);
  pinMode(io14,OUTPUT);
  pinMode(io15,OUTPUT);
  pinMode(io16,OUTPUT);
  pinMode(io17,OUTPUT);
  pinMode(io18,OUTPUT);
  pinMode(io19,OUTPUT);
  pinMode(io21,OUTPUT);
  pinMode(io22,OUTPUT);
  pinMode(io23,OUTPUT);
  pinMode(io25,OUTPUT);
  pinMode(io26,OUTPUT);
  pinMode(io27,OUTPUT);
  pinMode(io32,OUTPUT);
  pinMode(io33,OUTPUT);
}

void loop() {
  //delay(500);
  if(count==0){
    delay(5000);
    digitalWrite(io0,HIGH);
    digitalWrite(io1,HIGH);
    digitalWrite(io2,HIGH);
    digitalWrite(io4,HIGH);
    digitalWrite(io5,HIGH);
    digitalWrite(io12,HIGH);
    digitalWrite(io13,HIGH);
    digitalWrite(io14,HIGH);
    digitalWrite(io15,HIGH);
    digitalWrite(io16,HIGH);
    digitalWrite(io17,HIGH);
    digitalWrite(io18,HIGH);
    digitalWrite(io19,HIGH);
    digitalWrite(io21,HIGH);
    digitalWrite(io22,HIGH);
    digitalWrite(io23,HIGH);
    digitalWrite(io25,HIGH);
    digitalWrite(io26,HIGH);
    digitalWrite(io27,HIGH);
    digitalWrite(io32,HIGH);
    digitalWrite(io33,HIGH);
    count=1;
  }

  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    String status = String(digitalRead(1));
    String value = "32-1";
    String pinNo = value.substring(0,value.indexOf("-"));
    status = value.substring(value.length()-1,value.length()-0);
    snprintf (msg, MSG_BUFFER_SIZE, "Test publish to mqtt. GPIO %s is:%s", pinNo, status);
    Serial.println(msg);
    mqttClient.publish("herdi/device/temp", msg);
  }

  WiFiClient client = server.available(); // Listen for incoming clients
  if (client) { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime ) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) { // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c); // print it out the serial monitor
        header += c;
        if (c == '\n') { // if the byte is a newline character
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
              
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            // turns the GPIOs on and off
            writeHTML(io0, client, header);
            writeHTML(io1, client, header);
            writeHTML(io2, client, header);
            writeHTML(io4, client, header);
            writeHTML(io5, client, header);
            writeHTML(io12, client, header);
            writeHTML(io13, client, header);
            writeHTML(io14, client, header);
            writeHTML(io15, client, header);
            writeHTML(io16, client, header);
            writeHTML(io17, client, header);
            writeHTML(io18, client, header);
            writeHTML(io19, client, header);
            writeHTML(io21, client, header);
            writeHTML(io22, client, header);
            writeHTML(io23, client, header);
            writeHTML(io25, client, header);
            writeHTML(io26, client, header);
            writeHTML(io27, client, header);
            writeHTML(io32, client, header);
            writeHTML(io33, client, header);
            client.println("</body></html>");
              
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client_1-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("herdi/device/temp", "MQTT Server is Connected");
      // ... and resubscribe
      mqttClient.subscribe("herdi/device/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message+=String((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  
  int pinNo = message.substring(0,message.indexOf("-")).toInt();
  int value = message.substring(message.length()-1,message.length()-0).toInt();
  digitalWrite(pinNo, value);
  /*
  if ((char)payload[0] == '0') {
    digitalWrite(io1, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    //pref.putBool("Relay1", LOW);
  } else {
    digitalWrite(io1, HIGH); 
    //pref.putBool("Relay1", HIGH);
  }
  */
}