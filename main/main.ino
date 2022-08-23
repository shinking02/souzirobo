#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include <Wire.h>

RF24 radio(7, 8);
const byte address[][6] = {"00119", "00102"};
const int VOLTAGE_PIN = A5;
const int R_SENSOR_PIN = A0;
const int L_SENSOR_PIN = A1;
const int CENTER_SENSOR_PIN = A2;
const int L_BACK = 5;
const int L_FRONT = 6;
const int R_BACK = 9;
const int R_FRONT = 10;
int modeNumber;
int data[3];
float voltage = 1;
int r_stick, l_stick;


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
            manual();
            break;
        case 1:
            automatic();
            break;
        case 2:
            semi_auto();
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

void manual() {
    r_stick = map(data[1], 0, 1023, -255, 255);
    l_stick = map(data[2], 0, 1023, -255, 255);
    analogWrite(L_BACK, 0);
    analogWrite(L_FRONT, 0);
    analogWrite(R_BACK, 0);
    analogWrite(R_FRONT, 0);
    
    if(r_stick >= 10) {
        analogWrite(R_FRONT, r_stick);
    }else if(r_stick < 10) {
        analogWrite(R_BACK, abs(r_stick));
    }

    if(l_stick >= 10) {
        analogWrite(L_FRONT, l_stick);
    }else if(l_stick < 10) {
        analogWrite(L_BACK, abs(l_stick));
    }
}

void automatic() {
    int r_sensor = analogRead(R_SENSOR_PIN);
    int l_sensor = analogRead(L_SENSOR_PIN);
    int center_sensor = analogRead(CENTER_SENSOR_PIN);
    analogWrite(R_BACK, 0);
    analogWrite(L_BACK, 0);
    analogWrite(R_FRONT, 250);
    analogWrite(L_FRONT, 250);
    

    if(r_sensor > 300) {
        analogWrite(L_FRONT, 40);
    }
    if(l_sensor > 300) {
        analogWrite(R_FRONT, 40);
    }
    if(center_sensor > 350) {
        escape(r_sensor, l_sensor);
    }
}

void escape(int r, int l) {
    if(r > l) {
        analogWrite(R_BACK, 0);
        analogWrite(L_BACK, 100);
        analogWrite(R_FRONT, 100);
        analogWrite(L_FRONT, 0);
    }else {
        analogWrite(R_BACK, 100);
        analogWrite(L_BACK, 0);
        analogWrite(R_FRONT, 0);
        analogWrite(L_FRONT, 100);
    }
    delay(1800);
}

void semi_auto() {

}

