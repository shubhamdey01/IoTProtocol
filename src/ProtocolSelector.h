#ifndef PROTOCOL_SELECTOR_H
#define PROTOCOL_SELECTOR_H

#include "SensorRequirements.h"

// Protocols supported by the library
enum Protocol {
    MQTT_QOS0,
    MQTT_QOS1,
    MQTT_QOS2,
    COAP_NON
};

class ProtocolSelector {
public:

    // Select protocol based on priority and reliability
    Protocol select(Priority priority,
                    Reliability reliability);

    // Convert protocol enum to readable text
    const char* getProtocolName(Protocol protocol);
};

#endif