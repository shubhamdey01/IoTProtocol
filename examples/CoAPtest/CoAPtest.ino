#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

const char* ssid = "Moto G62";
const char* password = "asdfghjkl.";

IPAddress coap_server(10, 108, 77, 154);
const int coap_port = 5683;

WiFiUDP udp;
Coap coap(udp);

void callback_response(CoapPacket &packet, IPAddress ip, int port) {
    Serial.println("[CoAP Response]");

    char payload[packet.payloadlen + 1];

    memcpy(payload, packet.payload, packet.payloadlen);
    payload[packet.payloadlen] = '\0';

    Serial.println(payload);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    srand(micros());

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");

    Serial.print("ESP8266 IP: ");
    Serial.println(WiFi.localIP());

    coap.response(callback_response);
    coap.start();

    Serial.println("CoAP client started");

    const char* payload = "Hello from ESP8266 - NON";

    Serial.println("Sending CoAP NON...");

    uint16_t msgid = coap.send(
        coap_server,
        coap_port,
        "example_data",
        COAP_NONCON,
        COAP_PUT,
        NULL,
        0,
        (const uint8_t*)payload,
        strlen(payload)
    );

    Serial.print("Message ID: ");
    Serial.println(msgid);
}

void loop() {
    coap.loop();
    delay(100);
}