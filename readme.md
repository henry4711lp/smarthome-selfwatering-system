# Smarthome Self Watering System

This is a project for a smarthome self watering system that uses a microcontroller, sensors, and a water pump to automatically water plants. The system can be controlled remotely via a WiFi network and a MQTT broker.

## Hardware

- Microcontroller (e.g. ESP8266)
- Water pump
- Temperature sensor
- Soil moisture detector
- Water canister
- Planting pot

## Setup

1. Clone the repository and open the project in your Arduino IDE.
2. Update the code with the appropriate values for your WiFi network and MQTT broker.
    -   WiFi network name and password
    -   MQTT broker address, port, username, and password
    -   MQTT topics for the watering pumps
4. Upload the code to your microcontroller.
5. Connect the water pump, temperature sensor, soil moisture detector, and water canister to the microcontroller.
6. Place the planting pot in the water canister and fill it with soil and a plant.

## Usage

1. Once the code is uploaded and the hardware is set up, the smarthome self watering system will connect to the WiFi network and the MQTT broker.
2. The microcontroller will publish the status of the temperature and soil moisture sensors to the specified MQTT topic.
3. If the soil moisture is below a certain threshold, the microcontroller will activate the water pump to water the plant.
4. To manually control the watering system, send a message with the value "ON" or "OFF" to the MQTT topic for the water pump.

## Cost

The total cost of the components for the smarthome self watering system is approximately 47.42 EUR, plus the cost of the soil and plant. The individual components and their prices are as follows:

- Water Canister (11.99 EUR): https://www.obi.de/camping-zubehoer/happy-people-wasserkanister-faltbar/p/4323671
- Pump (2.25 EUR): https://www.amazon.de/gp/product/B088LQ4C1Z/ref=ox_sc_act_title_1?smid=A3BI8G9NTBZUKM&psc=1
- Temperature Sensor (3.70 EUR): https://www.amazon.de/gp/product/B07KNQJ3D7/ref=ox_sc_act_title_2?smid=A1X7QLRQH87QA3&psc=1
- Soil Moisture Detector (7.49 EUR): https://www.amazon.de/gp/product/B0B2DY4CZS/ref=ox_sc_act_title_4?smid=A1X7QLRQH87QA3&psc=1
- Microcontroller (5 EUR): https://www.az-delivery.de/products/nodemcu-lolin-v3-modul-mit-esp8266
- Planting Pot (16.99 EUR): https://www.obi.de/pflanzentoepfe-aussen/florus-blumenkasten-mediterran-mit-bewaesserungssystem-100-cm-anthrazit/p/4444444

## Dependencies

- ESP8266WiFi library (for ESP8266 microcontroller)
- PubSubClient library (for MQTT communication)
