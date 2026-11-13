#include "ProtocolSelector.h"

Protocol ProtocolSelector::select(Priority priority,
                                  Reliability reliability) {

    if (priority == PRIORITY_HIGH &&
        reliability == RELIABILITY_HIGH) {

        return MQTT_QOS2;
    }

    if (priority == PRIORITY_HIGH &&
        reliability == RELIABILITY_LOW) {

        return COAP_NON;
    }

    if (priority == PRIORITY_LOW &&
        reliability == RELIABILITY_HIGH) {

        return MQTT_QOS1;
    }

    // PRIORITY_LOW + RELIABILITY_LOW
    return MQTT_QOS0;
}


const char* ProtocolSelector::getProtocolName(Protocol protocol) {

    switch (protocol) {

        case MQTT_QOS0:
            return "MQTT QoS 0";

        case MQTT_QOS1:
            return "MQTT QoS 1";

        case MQTT_QOS2:
            return "MQTT QoS 2";

        case COAP_NON:
            return "CoAP NON";

        default:
            return "UNKNOWN";
    }
}