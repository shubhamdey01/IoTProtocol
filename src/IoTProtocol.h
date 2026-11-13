
#ifndef IOT_PROTOCOL_H
#define IOT_PROTOCOL_H

#include "SensorRequirements.h"
#include "ProtocolSelector.h"
#include "MQTTTransport.h"
#include "CoAPTransport.h"

class IoTProtocol {

private:
    Priority priority;
    Reliability reliability;
    PayloadMode payloadMode;

    Protocol selectedProtocol;

    ProtocolSelector selector;

    MQTTTransport mqtt;
    CoAPTransport coap;

public:
    void begin();

    void setPriority(Priority value);
    void setReliability(Reliability value);
    void setPayloadMode(PayloadMode value);

    Protocol selectProtocol();

    Protocol getProtocol();

    const char* getProtocolName();

    PayloadMode getPayloadMode();
    const char* getPayloadModeName();

    void beginMQTT(
        const char* server,
        uint16_t port,
        const char* clientId
    );

    void beginCoAP(
        IPAddress server,
        uint16_t port
    );

    // Non-blocking transmission
    bool send(
        const char* topicOrPath,
        const char* payload
    );

    bool mqttConnected();

    void mqttConnect();

    // MQTT performance statistics
    uint32_t getSubmittedCount();
    uint32_t getAcknowledgedCount();
    uint32_t getFailedCount();
    uint32_t getPendingCount();

    void resetMQTTStats();

    uint16_t getCoAPMessageId();
};

#endif
