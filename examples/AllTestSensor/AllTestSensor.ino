
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <IoTProtocol.h>

const char* ssid = "Moto G62";
const char* password = "asdfghjkl.";

const char* mqttServer = "10.108.77.154";
IPAddress coapServer(10, 108, 77, 154);

Adafruit_MPU6050 mpu;
IoTProtocol protocol;

uint32_t sequenceNumber = 0;

void setup() {

    Serial.begin(115200);
    delay(1000);

    Wire.begin(D2, D1);

    if (!mpu.begin()) {
        Serial.println("MPU6050 not detected!");
        while (1) delay(100);
    }

    Serial.println("MPU6050 detected!");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");

    protocol.begin();

    protocol.beginMQTT(mqttServer, 1883);
    protocol.beginCoAP(coapServer, 5683);

    Serial.println("Connecting MQTT...");

    protocol.mqttConnect();

    unsigned long start = millis();

    while (!protocol.mqttConnected() &&
           millis() - start < 10000) {
        delay(100);
    }

    Serial.println(
        protocol.mqttConnected()
        ? "MQTT connected"
        : "MQTT connection failed"
    );

    Serial.println("\nStarting MQTT QoS 1 experiment...");
}

void loop() {

    if (sequenceNumber >= 50) {
        Serial.println("\nExperiment completed.");

        while (true) {
            delay(1000);
        }
    }

    // Read sensor data
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // Prepare payload
    sequenceNumber++;

    char payload[100];

    snprintf(
        payload,
        sizeof(payload),
        "SEQ=%lu,Ax=%.2f,Ay=%.2f,Az=%.2f",
        (unsigned long)sequenceNumber,
        accel.acceleration.x,
        accel.acceleration.y,
        accel.acceleration.z
    );

    // MQTT QoS 1 requirements
    protocol.setPriority(PRIORITY_HIGH);
    protocol.setReliability(RELIABILITY_LOW);

    // Select protocol
    protocol.selectProtocol();

    Serial.println("\n---------------------------");

    Serial.print("Sequence: ");
    Serial.println(sequenceNumber);

    Serial.print("Selected protocol: ");
    Serial.println(protocol.getProtocolName());

    Serial.print("MQTT connected: ");
    Serial.println(
        protocol.mqttConnected() ? "YES" : "NO"
    );

    Serial.print("Payload: ");
    Serial.println(payload);

    // Measure protocol submission / acknowledgement time
    unsigned long startTime = micros();

    bool result = protocol.send(
        "example_data",
        payload
    );

    unsigned long endTime = micros();

    unsigned long latency = endTime - startTime;

    Serial.print("Transmission result: ");
    Serial.println(result ? "SUCCESS" : "FAILED");

    Serial.print("Send duration (microseconds): ");
    Serial.println(latency);

    Serial.print("Send duration (milliseconds): ");
    Serial.println(latency / 1000.0);

    delay(2000);
}
