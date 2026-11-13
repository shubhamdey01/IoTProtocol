#ifndef COAP_TRANSPORT_H
#define COAP_TRANSPORT_H

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

class CoAPTransport {

private:
    WiFiUDP udp;
    Coap coap;

    IPAddress server;
    uint16_t port;
    uint16_t messageId;
    uint16_t lastMessageId;

public:
    CoAPTransport();

    void begin(IPAddress serverAddress,
               uint16_t serverPort);

    bool publish(const char* path,
                 const char* payload);

    uint16_t getLastMessageId();
};

#endif