#ifndef MQTT_TRANSPORT_H
#define MQTT_TRANSPORT_H

#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>

class MQTTTransport {

private:
    AsyncMqttClient mqttClient;

    bool mqttConnected;

    // Maximum number of simultaneous outstanding MQTT publishes
    static const uint8_t MAX_PENDING_PUBLISHES = 32;

    struct PendingPublish {
        uint16_t packetId;
        bool active;
    };

    PendingPublish pendingPublishes[MAX_PENDING_PUBLISHES];

    // Performance counters
    uint32_t submittedCount;
    uint32_t acknowledgedCount;
    uint32_t failedCount;

    bool addPendingPublish(uint16_t packetId);
    void handlePublishAcknowledgement(uint16_t packetId);
    void clearPendingPublishes();

public:
    MQTTTransport();

    void begin(
        const char* server,
        uint16_t port,
        const char* clientId
    );

    bool connect();

    // Non-blocking publish
    bool publish(
        const char* topic,
        const char* payload,
        int qos
    );

    // Temporary compatibility function.
    // It now returns immediately and does NOT wait for acknowledgement.
    bool publishAndWait(
        const char* topic,
        const char* payload,
        int qos,
        unsigned long timeout
    );

    bool connected();

    // Performance statistics
    uint32_t getSubmittedCount();
    uint32_t getAcknowledgedCount();
    uint32_t getFailedCount();
    uint32_t getPendingCount();

    void resetStats();
};

#endif