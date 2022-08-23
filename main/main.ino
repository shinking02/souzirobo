#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include <Wire.h>

RF24 radio(7, 8);
const byte address[][6] = {"00119", "00102"};
const int VOLTAGE_PIN = A5;
const int L_BACK = 5;
const int L_FRONT = 6;
const int R_BACK = 9;
const int R_FRONT = 10;
int modeNumber;
int data[3];
float voltage = 1;

void setup() {
    radio.begin();
    radio.openWritingPipe(address[0]);
    radio.openReadingPipe(1, address[1]);
    radio.setPALevel(RF24_PA_MAX);
    radio.startListening();
    Serial.begin(9600);
}

void loop() {
    communication();
    voltage = analogRead(A5) / 51.4;
    switch(modeNumber) {
        case 0:

            break;
        case 1:

            break;
        case 2:

            break;
        default:
            break;
    }
}

//すべての通信処理
void communication() {
    delay(10);
    radio.stopListening();
    radio.write(&voltage, sizeof(voltage));
    delay(10);
    radio.startListening();
    if(radio.available()) {
        radio.read(&data, sizeof(data));
    }
}