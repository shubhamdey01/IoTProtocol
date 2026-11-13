#include "MQTTTransport.h"

MQTTTransport::MQTTTransport() {

    mqttConnected = false;

    submittedCount = 0;
    acknowledgedCount = 0;
    failedCount = 0;

    // Initialize all pending slots
    for (uint8_t i = 0; i < MAX_PENDING_PUBLISHES; i++) {
        pendingPublishes[i].packetId = 0;
        pendingPublishes[i].active = false;
    }

    // MQTT connection callback
    mqttClient.onConnect(
        [this](bool sessionPresent) {
            mqttConnected = true;
        }
    );

    // MQTT disconnection callback
    mqttClient.onDisconnect(
        [this](AsyncMqttClientDisconnectReason reason) {

            mqttConnected = false;

            // Any outstanding publishes are considered failed
            clearPendingPublishes();
        }
    );

    // MQTT publish acknowledgement callback
    mqttClient.onPublish(
        [this](uint16_t packetId) {
            handlePublishAcknowledgement(packetId);
        }
    );
}


// Configure MQTT broker
void MQTTTransport::begin(
    const char* server,
    uint16_t port,
    const char* clientId
) {

    mqttClient.setServer(server, port);
    mqttClient.setClientId(clientId);
}


// Initiate MQTT connection
bool MQTTTransport::connect() {

    if (mqttClient.connected()) {
        mqttConnected = true;
        return true;
    }

    mqttClient.connect();

    // Connection is asynchronous.
    // Actual connection status is updated by onConnect().
    return true;
}


// Add a packet ID to the pending list
bool MQTTTransport::addPendingPublish(uint16_t packetId) {

    for (uint8_t i = 0; i < MAX_PENDING_PUBLISHES; i++) {

        if (!pendingPublishes[i].active) {

            pendingPublishes[i].packetId = packetId;
            pendingPublishes[i].active = true;

            return true;
        }
    }

    return false;
}


// Handle acknowledgement received from MQTT broker
void MQTTTransport::handlePublishAcknowledgement(
    uint16_t packetId
) {

    for (uint8_t i = 0; i < MAX_PENDING_PUBLISHES; i++) {

        if (pendingPublishes[i].active &&
            pendingPublishes[i].packetId == packetId) {

            pendingPublishes[i].active = false;
            pendingPublishes[i].packetId = 0;

            acknowledgedCount++;

            return;
        }
    }
}


// Clear pending messages after disconnection
void MQTTTransport::clearPendingPublishes() {

    for (uint8_t i = 0; i < MAX_PENDING_PUBLISHES; i++) {

        if (pendingPublishes[i].active) {

            pendingPublishes[i].active = false;
            pendingPublishes[i].packetId = 0;

            failedCount++;
        }
    }
}


// Non-blocking MQTT publish
bool MQTTTransport::publish(
    const char* topic,
    const char* payload,
    int qos
) {

    // Check connection
    if (!mqttClient.connected()) {

        failedCount++;
        return false;
    }

    // Validate QoS
    if (qos < 0 || qos > 2) {

        failedCount++;
        return false;
    }

    // Check available acknowledgement-tracking space
    if (qos > 0 && getPendingCount() >= MAX_PENDING_PUBLISHES) {

        failedCount++;
        return false;
    }

    // Submit MQTT message
    uint16_t packetId = mqttClient.publish(
        topic,
        qos,
        false,
        payload
    );

    if (packetId == 0) {

        failedCount++;
        return false;
    }

    // Track QoS 1 and QoS 2 messages
    if (qos > 0) {

        if (!addPendingPublish(packetId)) {

            failedCount++;
            return false;
        }
    }

    submittedCount++;

    // Return immediately.
    // No acknowledgement waiting occurs here.
    return true;
}


// Temporary compatibility function
bool MQTTTransport::publishAndWait(
    const char* topic,
    const char* payload,
    int qos,
    unsigned long timeout
) {

    (void)timeout;

    // Now behaves exactly like non-blocking publish().
    return publish(topic, payload, qos);
}


// Check connection
bool MQTTTransport::connected() {

    return mqttClient.connected();
}


// Return submitted message count
uint32_t MQTTTransport::getSubmittedCount() {

    return submittedCount;
}


// Return acknowledged message count
uint32_t MQTTTransport::getAcknowledgedCount() {

    return acknowledgedCount;
}


// Return failed message count
uint32_t MQTTTransport::getFailedCount() {

    return failedCount;
}


// Return number of outstanding messages
uint32_t MQTTTransport::getPendingCount() {

    uint32_t count = 0;

    for (uint8_t i = 0; i < MAX_PENDING_PUBLISHES; i++) {

        if (pendingPublishes[i].active) {
            count++;
        }
    }

    return count;
}


// Reset performance counters
void MQTTTransport::resetStats() {

    submittedCount = 0;
    acknowledgedCount = 0;
    failedCount = 0;
}