
#include "IoTProtocol.h"

void IoTProtocol::begin() {

    priority = PRIORITY_LOW;
    reliability = RELIABILITY_LOW;
    payloadMode = PAYLOAD_RAW;

    selectedProtocol = MQTT_QOS0;
}

void IoTProtocol::setPriority(Priority value) {
    priority = value;
}

void IoTProtocol::setReliability(Reliability value) {
    reliability = value;
}

void IoTProtocol::setPayloadMode(PayloadMode value) {
    payloadMode = value;
}

Protocol IoTProtocol::selectProtocol() {

    selectedProtocol =
        selector.select(priority, reliability);

    return selectedProtocol;
}

Protocol IoTProtocol::getProtocol() {
    return selectedProtocol;
}

const char* IoTProtocol::getProtocolName() {
    return selector.getProtocolName(selectedProtocol);
}

PayloadMode IoTProtocol::getPayloadMode() {
    return payloadMode;
}

const char* IoTProtocol::getPayloadModeName() {

    switch (payloadMode) {

        case PAYLOAD_RAW:
            return "RAW";

        case PAYLOAD_SEMANTIC:
            return "SEMANTIC";

        default:
            return "UNKNOWN";
    }
}

// --------------------------------------------------
// MQTT CONFIGURATION
// --------------------------------------------------

void IoTProtocol::beginMQTT(
    const char* server,
    uint16_t port,
    const char* clientId
) {

    mqtt.begin(server, port, clientId);
}

// --------------------------------------------------
// COAP CONFIGURATION
// --------------------------------------------------

void IoTProtocol::beginCoAP(
    IPAddress server,
    uint16_t port
) {

    coap.begin(server, port);
}

// --------------------------------------------------
// NON-BLOCKING TRANSMISSION
// --------------------------------------------------

bool IoTProtocol::send(
    const char* topicOrPath,
    const char* payload
) {

    selectProtocol();

    switch (selectedProtocol) {

        case MQTT_QOS0:

            return mqtt.publish(
                topicOrPath,
                payload,
                0
            );

        case MQTT_QOS1:

            return mqtt.publish(
                topicOrPath,
                payload,
                1
            );

        case MQTT_QOS2:

            return mqtt.publish(
                topicOrPath,
                payload,
                2
            );

        case COAP_NON:

            return coap.publish(
                topicOrPath,
                payload
            );

        default:
            return false;
    }
}

// --------------------------------------------------
// MQTT CONNECTION STATUS
// --------------------------------------------------

bool IoTProtocol::mqttConnected() {
    return mqtt.connected();
}

void IoTProtocol::mqttConnect() {
    mqtt.connect();
}

// --------------------------------------------------
// MQTT PERFORMANCE STATISTICS
// --------------------------------------------------

uint32_t IoTProtocol::getSubmittedCount() {
    return mqtt.getSubmittedCount();
}

uint32_t IoTProtocol::getAcknowledgedCount() {
    return mqtt.getAcknowledgedCount();
}

uint32_t IoTProtocol::getFailedCount() {
    return mqtt.getFailedCount();
}

uint32_t IoTProtocol::getPendingCount() {
    return mqtt.getPendingCount();
}

void IoTProtocol::resetMQTTStats() {
    mqtt.resetStats();
}

// --------------------------------------------------
// COAP MESSAGE ID
// --------------------------------------------------

uint16_t IoTProtocol::getCoAPMessageId() {
    return coap.getLastMessageId();
}
