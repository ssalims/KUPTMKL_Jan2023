/*
UPTMKL 19 Jan 2023 4.40pm
NODE-RED with ESP32s & DHT11

 Basic ESP32s MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP32s board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "sensor1" every two seconds
  - subscribes to the topic "server", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "server" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP32 board, (using Arduino IDE 2.0.3):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP32"
  - Select your esp32 by Espressif Systems in "Tools -> Board"
*/

//#include <ESP8266WiFi.h>
#include <WiFi.h> //if you are using esp32
#include <PubSubClient.h>

//DHT11 setup
#include "DHT.h"
#define DHTPIN 22   
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//  WiFi information
const char* ssid = "IP7+";//"........";
const char* password = "12345678a";//"........";

// MQTT Broker information
const char* mqtt_server = "172.20.10.2";//"192.168.0.166"; // // MQTT's IP Address
const int mqtt_port = 1883; // MQTT's port
const char *this_topic = "sensor1"; //Topic or ID for this microcontroller
const char *server_topic = "server"; //Topic or ID for server



WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  ////WiFi.mode(WIFI_STA);
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

void callback(char* server_topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(server_topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32S:Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(this_topic, "hello world");
      // ... and resubscribe
      client.subscribe(server_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  dht.begin();
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

//////////////
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit = true)
float f = dht.readTemperature(true);
//////////////

char fahrenheitValue[15],celciusValue[15], humidValue[15];
  dtostrf(f, 4, 2, fahrenheitValue); // convert float to char 
  dtostrf(h, 4, 2, humidValue); // convert float to char 
  dtostrf(t, 4, 2, celciusValue); // convert float to char 

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish(this_topic, msg); //send to Aedes Broker
    client.publish("stationA/celc", celciusValue); 
    client.publish("stationA/fah", fahrenheitValue);
    client.publish("stationA/humid", humidValue); 
 
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.println(f);

  }
  
}
