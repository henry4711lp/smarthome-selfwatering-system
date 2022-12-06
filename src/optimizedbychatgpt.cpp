#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <RelayModule.h>

//Relays
#define RELAY_PIN   D2
#define RELAY_PIN2   D3
#define INVERT_RELAY true
RelayModule* relay;
RelayModule* relay2;

// WiFi
const char *ssid = ""; // Enter your WiFi name
const char *password = "";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "";
const char *topic = "";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

//LED Blink
#define BLINK_PERIOD 750
long lastBlinkMillis;
boolean ledState;

//Reconnections
#define WIFI_RECONNECT_PERIOD 600000
#define MQTT_RECONNECT_PERIOD 300000
long lastWIFIReconnectMillis;
long lastMQTTReconnectMillis;

// Read Sensor

int readSensor(int pin) {
  return digitalRead(pin);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String message = (char*)payload;
  String topicStr = topic;

  Serial.println(topicStr + " was received");
  Serial.println(message + " was received");

  if(topicStr == "watering/pump1")
  {
    if(message == "ON")
    {
      Serial.println("Pump 1 on");
      relay->on();
      client.publish("watering/pump1/status", "ON");
    }
    else if(message == "OFF")
    {
      Serial.println("Pump 1 off");
      relay->off();
      client.publish("watering/pump1/status", "OFF");
    }
  }
  else if(topicStr == "watering/pump2")
  {
    if(message == "ON")
    {
      Serial.println("Pump 2 on");
      relay2->on();
      client.publish("watering/pump2/status", "ON");
    }
    else if(message == "OFF")
    {
      Serial.println("Pump 2 off");
      relay2->off();
      client.publish("watering/pump2/status", "OFF");
    }
  }
}
void wifi_connector()
{
  WiFi.disconnect();
  delay(3000);
  WiFi.setAutoReconnect(true);
  WiFi.setHostname("wateringsystem");
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi...");
  }
  String IP = WiFi.localIP().toString();
  Serial.println("Connected to the WiFi network " + WiFi.SSID() +" and IP " + IP + " and Hostname " + WiFi.getHostname());
}

void mqtt_connector()
{
  client.disconnect();
  delay(3000);
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
    while (!client.connected()) {
      String client_id = "watering-";
      client_id += String(WiFi.macAddress());
      Serial.println("Connecting to MQTT broker...");
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Connected to MQTT broker");
        client.subscribe(topic);
      } else {
        Serial.println("Connection to MQTT broker failed");
        delay(5000);
      }
    }
}
void setup()
{
  Serial.begin(115200);
  // Initialize relays
  relay = new RelayModule(RELAY_PIN, INVERT_RELAY);
  relay2 = new RelayModule(RELAY_PIN2, INVERT_RELAY);

  // Connect to WiFi
  wifi_connector();
}

void loop()
{
  // Check if we need to reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - lastWIFIReconnectMillis > WIFI_RECONNECT_PERIOD) {
      lastWIFIReconnectMillis = millis();
      wifi_connector();
    }
  }

  // Check if we need to reconnect to MQTT broker
  if (!client.connected()) {
    if (millis() - lastMQTTReconnectMillis > MQTT_RECONNECT_PERIOD) {
      lastMQTTReconnectMillis = millis();
      mqtt_connector();
    }
  }

  // Handle incoming messages from MQTT broker
  client.loop();

  // Blink LED
  if (millis() - lastBlinkMillis > BLINK_PERIOD) {
    lastBlinkMillis = millis();
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}
