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



// Read Sensor 1

int readsensor1(){
  int digitalValue = digitalRead(D0);
  /*
  Serial.print("Digital 1 reads: ");
  Serial.print(digitalValue);
  Serial.print("\n\n");
  */
  return digitalValue;
}

int readsensor2(){
  int digitalValue = digitalRead(D1);
  /*
  Serial.print("Digital 2 reads: ");
  Serial.print(digitalValue);
  Serial.print("\n\n");
  */
  return digitalValue;
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
      Serial.println("Pumpe an");
      relay->on();
      client.publish("watering/pump1/status", "ON");
    }
    else if(message == "OFF")
    {
      Serial.println("Pumpe aus");
      relay->off();
      client.publish("watering/pump1/status", "OFF");
    }
  }
  if(topicStr == "watering/pump2")
  {
    if(message == "ON")
    {
      Serial.println("Pumpe an");
      relay2->on();
      client.publish("watering/pump2/status", "ON");
    }
    else if(message == "OFF")
    {
      Serial.println("Pumpe aus");
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
      Serial.printf("The client %s connects to the mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
    }
  // publish and subscribe
  client.publish("watering/status", "Online");
  client.subscribe(topic);
  client.subscribe("watering/pump1");
  client.subscribe("watering/pump2");
}

void setup() {
  relay = new RelayModule(RELAY_PIN, INVERT_RELAY);
  relay2 = new RelayModule(RELAY_PIN2, INVERT_RELAY);
  pinMode(LED_BUILTIN, OUTPUT);
  // Set software serial baud to 115200;
  Serial.begin(115200);
  wifi_connector();
  mqtt_connector();
}



bool senddata1(){
  //Serial.print("Sending MoistSensor 1 \n");
  int sensordata=readsensor1();
  if(sensordata == 1)
  {
  client.publish("watering/Moist1", "trocken");
  }
  else if (sensordata == 0)  
  {
    client.publish("watering/Moist1", "feucht");
  }
  return true;
}
bool senddata2(){
  //Serial.print("Sending MoistSensor 2 \n");
  int sensordata=readsensor2();
  if(sensordata == 1)
  {
  client.publish("watering/Moist2", "trocken");
  }
  else if (sensordata == 0)  
  {
    client.publish("watering/Moist2", "feucht");
  }
  return true;
}

void loop() {
  if(WiFi.isConnected() == true)
  {
    if(client.connected() == true)
    {
      client.loop();
      
      // blink LED
      long currentMillis = millis();
      if (currentMillis - lastBlinkMillis > BLINK_PERIOD){
        /*
        Serial.print("\n");
        Serial.print("---------------------------------\n");
        Serial.print("-------------New Tick------------\n");
        Serial.print("---------------------------------\n");
        Serial.print("\n");
        */
        Serial.println("Running...");
        digitalWrite(LED_BUILTIN, ledState);
        ledState = !ledState;
        lastBlinkMillis = currentMillis;
        if(senddata1() == false){Serial.println("Failed to send data");}
      else{}
      if(senddata2() == false){Serial.println("Failed to send data");}
      else{}
      }
      if(currentMillis - lastMQTTReconnectMillis > MQTT_RECONNECT_PERIOD){
        client.publish("watering/status", "MQTT reconnecting...");
        Serial.println("5 Minutes passed reconnecting MQTT");
        mqtt_connector();
        lastMQTTReconnectMillis = currentMillis;
      }
      if(currentMillis - lastWIFIReconnectMillis > WIFI_RECONNECT_PERIOD){
        client.publish("watering/status", "WIFI reconnecting...");
        Serial.println("10 Minutes passed reconnecting Wifi");
        wifi_connector();
        lastWIFIReconnectMillis = currentMillis;
      }
    }
    else{
      mqtt_connector();
    }
  }
  else
  {
    wifi_connector();
  }
  
}