#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <MsTimer2.h>
#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);
RF24 radio(7, 8);
const byte address[][6] = {"00119", "00102"};

const int RX_PIN = A1;
const int LX_PIN = A2;
const int SW0_PIN = 2; //モードセレクト
const int SW1_PIN = 3;
const int SW2_PIN = 4;
int modeNumber;
String modeName[] = {"manual", "auto", "semi-auto"};
int rx, lx;
int voltage = 0;
int data[3] = {0};
int errorFlag = 0;

void setup() {
    pinMode(RX_PIN, INPUT);
    pinMode(LX_PIN, INPUT);
    pinMode(SW0_PIN, INPUT);
    pinMode(SW1_PIN, INPUT);
    pinMode(SW2_PIN, INPUT);
    modeNumber = EEPROM.read(0x000);
    radio.begin();
    radio.openWritingPipe(address[1]);
    radio.openReadingPipe(1, address[0]);
    radio.setPALevel(RF24_PA_MIN);
    radio.setAutoAck(true);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    display.display();
    delay(1000);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    radio.stopListening();

    Serial.begin(9600);

}
