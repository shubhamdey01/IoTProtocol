#include "CoAPTransport.h"

CoAPTransport::CoAPTransport()
    : coap(udp) {

    port = 5683;
    messageId = 1;
    lastMessageId = 0;
}

void CoAPTransport::begin(IPAddress serverAddress,
                          uint16_t serverPort) {

    server = serverAddress;
    port = serverPort;

    coap.start();
}

bool CoAPTransport::publish(const char* path,
                            const char* payload) {

    uint16_t msgid = coap.send(
        server,
        port,
        path,
        COAP_NONCON,
        COAP_PUT,
        NULL,
        0,
        (const uint8_t*)payload,
        strlen(payload),
        COAP_NONE,
        messageId++
    );

    if (msgid == 0) {
        return false;
    }

    lastMessageId = msgid;

    return true;
}

uint16_t CoAPTransport::getLastMessageId() {
    return lastMessageId;
}