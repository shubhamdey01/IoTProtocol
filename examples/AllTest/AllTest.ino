#include <ESP8266WiFi.h>
#include <IoTProtocol.h>

const char* ssid = "Moto G62";
const char* password = "asdfghjkl.";

const char* mqttServer = "10.108.77.154";
IPAddress coapServer(10, 108, 77, 154);

IoTProtocol protocol;

void setup() {

    Serial.begin(115200);
    delay(1000);

    WiFi.begin(ssid, password);

    Serial.print("Connecting");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");

    Serial.print("ESP8266 IP: ");
    Serial.println(WiFi.localIP());

    // Initialize protocol system
    protocol.begin();

    // Initialize transports
    protocol.beginMQTT(mqttServer, 1883);
    protocol.beginCoAP(coapServer, 5683);

    Serial.println("IoTProtocol started");

    // Connect to MQTT
    Serial.println("Connecting to MQTT...");

    protocol.mqttConnect();

    unsigned long startTime = millis();

    while (!protocol.mqttConnected() &&
           millis() - startTime < 10000) {

        delay(100);
        Serial.print(".");
    }

    Serial.println();

    if (protocol.mqttConnected()) {
        Serial.println("MQTT connected");
    } else {
        Serial.println("MQTT connection FAILED");
    }

    // Run all protocol-selection tests

    testRequirement(
        PRIORITY_HIGH,
        RELIABILITY_HIGH,
        "HIGH_HIGH"
    );

    testRequirement(
        PRIORITY_HIGH,
        RELIABILITY_LOW,
        "HIGH_LOW"
    );

    testRequirement(
        PRIORITY_LOW,
        RELIABILITY_HIGH,
        "LOW_HIGH"
    );

    testRequirement(
        PRIORITY_LOW,
        RELIABILITY_LOW,
        "LOW_LOW"
    );
}

void testRequirement(Priority priority,
                     Reliability reliability,
                     const char* name) {

    protocol.setPriority(priority);
    protocol.setReliability(reliability);

    Protocol selected = protocol.selectProtocol();

    Serial.println();
    Serial.print("Test: ");
    Serial.println(name);

    Serial.print("Selected protocol: ");
    Serial.println(protocol.getProtocolName());

    bool result;

    if (selected == COAP_NON) {

        result = protocol.send(
            "example_data",
            name
        );

    } else {

        result = protocol.send(
            "iotprotocol/test",
            name
        );
    }

    Serial.print("Send result: ");

    if (result) {
        Serial.println("SUCCESS");
    } else {
        Serial.println("FAILED");
    }

    if (selected == COAP_NON && result) {
      Serial.print("CoAP Message ID: ");
      Serial.println(protocol.getCoAPMessageId());
    }

    delay(2000);
}

void loop() {
}