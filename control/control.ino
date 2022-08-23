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
float voltage = 0;
int data[3] = {0, 512, 512};

void setup() {
    pinMode(RX_PIN, INPUT);
    pinMode(LX_PIN, INPUT);
    pinMode(SW0_PIN, INPUT);
    pinMode(SW1_PIN, INPUT);
    pinMode(SW2_PIN, INPUT);
    modeNumber = EEPROM.read(0x000);
    data[0] = modeNumber;
    radio.begin();
    radio.openWritingPipe(address[1]);
    radio.openReadingPipe(1, address[0]);
    radio.setPALevel(RF24_PA_MAX);
    radio.setAutoAck(true);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    display.display();
    delay(1000);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    radio.stopListening();
    Serial.begin(9600);

//起動時接続待ち処理
    while(!radio.write(&data, sizeof(data))) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("waiting connection");
        display.display();
        delay(200);
        for(int i = 0; i < 3; i++) {
            display.print(".");
            display.display();
            delay(200);
        }
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("connected!");
    display.display();
    delay(2000);
    dispHome();
    MsTimer2::set(2000, dispHome);
    MsTimer2::start();
}

void loop() {
    rx = analogRead(RX_PIN);
    lx = analogRead(LX_PIN);
    data[0] = modeNumber;
    data[1] = rx;
    data[2] = lx;
    communication();
    checkSwitch();
}

//すべての通信処理
void communication() {
    delay(10);
    radio.stopListening();
    if(!radio.write(&data, sizeof(data))) {
        Serial.println("sendError");
    }
    delay(10);
    radio.startListening();
    if(radio.available()) {
        radio.read(&voltage, sizeof(voltage));
    }
}

//スイッチ監視
void checkSwitch() {
    int sw0;
    sw0 = digitalRead(SW0_PIN);
        if(sw0 == LOW && modeNumber == 2) {
            modeNumber = 0;
            EEPROM.write(0x000, modeNumber);
            dispHome();
        }else if(sw0 == LOW) {
            modeNumber ++;
            EEPROM.write(0x000, modeNumber);
            dispHome();
        }
}

//oled描画
void dispHome() {
    interrupts();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("mode: ");
    display.print(modeName[modeNumber]);
    display.setCursor(0, 8);
    display.print("voltage: ");
    display.print(voltage);
    display.print("v");
    display.display();
}