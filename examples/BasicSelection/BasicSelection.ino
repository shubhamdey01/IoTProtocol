#include <ESP8266WiFi.h>
#include <MQTTTransport.h>
#include <IoTProtocol.h>

const char* ssid = "Moto G62";
const char* password = "asdfghjkl.";

const char* mqtt_server = "10.108.77.154";
const uint16_t mqtt_port = 1883;

IoTProtocol protocol;
MQTTTransport mqtt;


void setup() {

    Serial.begin(115200);
    delay(1000);

    // -------------------------
    // Connect to WiFi
    // -------------------------

    Serial.print("Connecting to WiFi");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());


    // -------------------------
    // Initialize protocol
    // -------------------------

    protocol.begin();

    protocol.setPriority(PRIORITY_LOW);
    protocol.setReliability(RELIABILITY_LOW);
    protocol.setPayloadMode(PAYLOAD_RAW);


    // -------------------------
    // Select protocol
    // -------------------------

    Protocol selected = protocol.selectProtocol();

    Serial.println();
    Serial.println("Requirements:");
    Serial.println("Priority: HIGH");
    Serial.println("Reliability: HIGH");
    Serial.println("Payload: RAW");

    Serial.print("Selected protocol: ");
    Serial.println(protocol.getProtocolName());


    // -------------------------
    // Initialize MQTT
    // -------------------------

    mqtt.begin(mqtt_server, mqtt_port);

    Serial.println();
    Serial.println("Connecting to MQTT...");

    mqtt.connect();

    delay(2000);


    if (!mqtt.connected()) {

        Serial.println("MQTT connection failed");
        return;
    }

    Serial.println("MQTT connected");


    // -------------------------
    // Use selected MQTT QoS
    // -------------------------

    int qos = 0;

    if (selected == MQTT_QOS0) {
        qos = 0;
    }
    else if (selected == MQTT_QOS1) {
        qos = 1;
    }
    else if (selected == MQTT_QOS2) {
        qos = 2;
    }

    Serial.print("Using MQTT QoS: ");
    Serial.println(qos);


    // -------------------------
    // Publish
    // -------------------------

    if (mqtt.publish(
            "iotprotocol/test",
            "Hello from IoTProtocol",
            qos)) {

        Serial.println("Message sent");

    } else {

        Serial.println("Message failed");
    }
}


void loop() {
}