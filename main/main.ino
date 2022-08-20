#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include <Wire.h>

RF24 radio(7, 8);
const byte address[][6] = {"00119", "00102"};
int modeNumber;
int data[3];
int r = 1;

void setup() {
    radio.begin();
    radio.openWritingPipe(address[0]);
    radio.openReadingPipe(1, address[1]);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    Serial.begin(9600);
}

void loop() {
    if(radio.available()) {
        Serial.println("recv");
    }
}