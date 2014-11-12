#include "ClickEncoder.h"
#include "TEA5767.h"
#include "DIY_Radio.h"
#include <Wire.h>
#include "TimerOne.h"

// Custom classes and objects
TEA5767 radio;
RGBHex  ledArray;
Mode    currentMode;

// objecs from library
ClickEncoder *encoder;

// Radio variables
int16_t last,frequency = 9910;

//LED variables
volatile int count = 0;

//Interrupt Service Routines
void timerIsr() {
    encoder->service();
}

ISR(TIMER2_COMPA_vect){ // will happen at ~100 Hz (100.16025641) (16000000 /1024/156) [clock / prescaler / count]
    count = count > 100 ? 0: count+1;
    if (count == 0)
        LED_SWITCH; // switch the LED every second

    if (radio.isScanning && count % 20){ // if the radio is scanning, check for end of band and/or found station 5 times per second
        ledArray.updateRotation();
        if(radio.checkEnd()) { // if the end of the band has been reached
            radio.restartScan(); // restart the scan (from top or bottom, TBD by function)
        }
        else if (radio.ready){ // if the station has been found (just updated data from radio.checkEnd())
            radio.stopScanning();
        }
    }
}

void setup() {
    Serial.begin(115200);
    encoder = new ClickEncoder(A1, A0, A2);
    Wire.begin();
    Timer1.initialize(1000); // run timerIsr every 1ms
    Timer1.attachInterrupt(timerIsr); 

    //pinmode settings
    pinMode(ledPin, OUTPUT);
    pinMode(BLUETOOTH_FET, OUTPUT);
    pinMode(SPEAKER_FET, OUTPUT);
    pinMode(RADIO_FET, OUTPUT);
    pinMode(AMP_FET, OUTPUT);
    last = -1;

    // setup timer2 for interrupt to blink or rotate
    cli(); // disable global interrupts 
    TCCR2B = 0; // reset control regesters
    TCCR2A = 0;

    OCR2A = 0x9C; // will count to 156 before overflow, will create 100 Hz overflow at 1024 prescalar
    TCCR2A |= (1 << WGM21)|(1 << WGM20);
    TCCR2B |= (1 << WGM22);
    TCCR2B |= (1 << CS22); // timer2 1024 prescalar
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS20);
    TIMSK2 |= (1 << OCIE2A); // enable output compare interrupts on 2A
    sei(); // enable global interrupts
}

void loop() {
    frequency += encoder->getValue();    
    ClickEncoder::Button b = encoder->getButton();
    switch (b) {
        case ClickEncoder::Clicked:
            Serial.println("Button: Pressed");
            Serial.print("Signal Level is: ");
            Serial.println(radio.getSignal());
            break;
        case ClickEncoder::Held:
            Serial.println("Button: Held")
            if (frequency != last) {
                // put scanning code in here
                if (frequency > last){
                    radio.scan(1);// scan up
                }
                else{
                    radio.scan(0);// scan down
                }
            }
            break;
        case ClickEncoder::Released:
            Serial.println("Button: Released");
            break;
        case ClickEncoder::DoubleClicked:
            Serial.println("ClickEncoder::DoubleClicked");
            encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
            Serial.print("  Acceleration is ");
            Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
            cycleMode();
            Serial.print("Changed to: ");
            Serial.println(currentMode);
            break;
        case ClickEncoder::Open:
            if (frequency != last) {
                if (frequency < 8750){
                    frequency = 10800;
                }
                else if (frequency > 10800){
                    frequency = 8750;
                }
                last = frequency;
                Serial.print("New frequency: ");
                Serial.println(frequency);
                radio.SetFrequency(frequency);
            }
            break;
    }
}

void cycleMode(){
    switch (currentMode) {
        case standby:
            currentMode = radio;
            // turn amplifier on
            digitalWrite(AMP_FET, ON);
            radio.unstandby();// turn radio on
            break;
        case radio:
            currentMode = bluetooth;
            radio.standby();
            digitalWrite(BLUETOOTH_FET, ON);
            // turn bluetooth on
            break;
        case bluetooth:
            currentMode = aux;
            digitalWrite(BLUETOOTH_FET, OFF);
            // turn bluetooth off, aux will just use amplifier
            break;
        case aux:
            currentMode = standby;
            // turn amplifier off
            digitalWrite(AMP_FET, ON);
            break;
    }
}