#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid ="IKRAME";// "REPLACE_WITH_YOUR_SSID";
const char* password ="ikrame1997iko";// "REPLACE_WITH_YOUR_PASSWORD";
// Add your MQTT Broker IP address, example:
const char* mqtt_server ="test.mosquitto.org";//"192.168.1.6"; 
//"YOUR_MQTT_BROKER_IP_ADDRESS";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float current = 0;
// LED Pin
const int ledPin = 2;
void setup() {
 Serial.begin(115200);
 // default settings
 setup_wifi();
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 pinMode(ledPin, OUTPUT);
}
void setup_wifi() {
 delay(10);
 // We start by connecting to a WiFi network
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* message, unsigned int length) {
 Serial.print("Message arrived on topic: ");
 Serial.print(topic);
 Serial.print(". Message: ");
 String messageTemp;
 for (int i = 0; i < length; i++) {
 Serial.print((char)message[i]);
 messageTemp += (char)message[i];
 }
 Serial.println();
 // Feel free to add more if statements to control more GPIOs with MQTT

// If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
 // Changes the output state according to the message
 if (String(topic) == "light") 
 {
 Serial.print("Light: Changing output to ");
 if(messageTemp == "on"){
 Serial.println("on");
 digitalWrite(ledPin, HIGH);
 }
 else if(messageTemp == "off"){
 Serial.println("off");
 digitalWrite(ledPin, LOW);
 }
 }
}
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266Client")) {
 Serial.println("connected");
 // Subscribe
 client.subscribe("light");
 } else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println(" try again in 5 seconds");
 // Wait 5 seconds before retrying
 delay(5000);
 }
 }
}
void loop() {
 if (!client.connected()) {
 reconnect();
 }
 client.loop();
 long now = millis();

 if (now - lastMsg > 1000) {
 lastMsg = now;
 
 // Current in Ampere
 current = analogRead(A0); 
 
 // Convert the value to a char array
 char currentString[8];
 dtostrf(current, 1, 2, currentString);
 Serial.print("Current: ");
 Serial.println(currentString);
 client.publish("current", currentString);
 
}
}
