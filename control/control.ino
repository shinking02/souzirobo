    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    #include <EEPROM.h>
    #include <Wire.h>
    #include <Adafruit_SSD1306.h>
    #define OLED_RESET 4

    Adafruit_SSD1306 display(OLED_RESET);
    RF24 radio(7, 8);
    const byte address[][6] = {"00119", "00102"};

    const int RX_PIN = A1;
    const int LX_PIN = A2;
    const int SW0_PIN = 5; //モードセレクト
    const int SW1_PIN = 6;
    const int SW2_PIN = 7;
    int modeNumber;
    String modeName[] = {"manual", "auto", "semi-auto"};
    int rx, lx;
    int voltage;
    bool connection = false;
    const int connectionKey = 119;

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
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
        display.display();
        delay(1000);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.clearDisplay();
        Serial.begin(9600);

        while(!connection) {
            int read = 0;
            display.setCursor(0,0);
            display.print("waiting connection");
            display.display();
            radio.startListening();
            delay(100);
            for(int i = 0; i < 4; i++) {
                if(radio.available()) {
                    Serial.println("nano:recv");
                    radio.read(read, sizeof(read));
                    if(read == connectionKey) {
                        connection = true;
                    }
                }
                if(i == 0) {
                    continue;
                }else {
                    display.print(".");
                    display.display();
                }
                delay(100);
            }
            display.clearDisplay();
        }
        
    }

    void dispHome() {
        for(int i = 0; i < 100; i++) {
            display.println(i);
            display.display();
            delay(1000);
        }

    }

    void checkMode() {
        int sw0;
        sw0 = digitalRead(SW0_PIN);
        if(sw0 == LOW && modeNumber == 2) {
            modeNumber = 0;
        }else if(sw0 == LOW) {
            modeNumber ++;
        }
        EEPROM.write(0x000, modeNumber);
    }



    void sendData() {
        radio.stopListening();
        radio.write(rx, sizeof(rx));
        radio.write(lx, sizeof(lx));
    }

    void recvData() {
        radio.startListening();
        radio.read(voltage, sizeof(voltage));
    }

    void loop() {
        rx = analogRead(RX_PIN);
        lx = analogRead(LX_PIN);
        checkMode();
        recvData();
        sendData();
        dispHome();
    }