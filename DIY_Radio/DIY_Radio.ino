#include "ClickEncoder.h"
#include "TEA5767.h"
#include "DIY_Radio.h"
#include <Wire.h>
#include "TimerOne.h"

// Custom classes and objects
TEA5767 radio;
// RGBHex  ledArray;
Mode    currentMode;

// objecs from library
ClickEncoder *encoder;

// Radio variables
int16_t lastFrequency,frequency = 9910;
int16_t velocity = 0;
byte old_mute=1;
byte mute=0;
long loopnum = 0;
byte old_signal_level=1;
byte signal_level=0;
bool startedScanning = false;
bool canSetFrequency = true;

//LED variables
volatile int count = 0;

//Interrupt Service Routines
void timerIsr() {
    encoder->service();
}


ISR(TIMER2_COMPA_vect){ // should happen at ~100 hz (16000000 /1024/156) [clock / prescaler / count]
    count = count>100?0: count+1;
    if (count == 0)
        LED_SWITCH; // switch the LED every second

    if (radio.isScanning && count ==0){ // if the radio is scanning, check for end of band and/or found station 6 times per second
        // ledArray.updateRotation();
        if(radio.checkEnd()) { // if the end of the band has been reached
            radio.restartScan(); // restart the scan (from top or bottom, TBD by function)
        }
        else if (radio.ready){ // if the station has been found (just updated data from the if statement)
            Serial.println("frequency is found!");
            radio.stopScanning();
        }
        // set this frequency to frequency from chip
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
    pinMode(8, OUTPUT);
    pinMode(BLUETOOTH_FET, OUTPUT);
    pinMode(SPEAKER_FET, OUTPUT);
    pinMode(RADIO_FET, OUTPUT);
    pinMode(AMP_FET, OUTPUT);

    lastFrequency = -1;

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
    velocity = encoder->getValue();
    frequency += velocity;
    if (startedScanning && velocity == 0){
        canSetFrequency = true;
    }
    
    
    ClickEncoder::Button b = encoder->getButton();
    switch (b) {
        case ClickEncoder::Open:
            if (frequency != lastFrequency) {
                if (frequency < 8750){
                    frequency = 10800;
                }
                else if (frequency > 10800){
                    frequency = 8750;
                }
                lastFrequency = frequency;
                Serial.print("New frequency: ");
                Serial.println(frequency);
                radio.setFrequency(frequency);
            }
            break;
        case ClickEncoder::Clicked:
            Serial.print("Signal Level is: ");
            Serial.println(radio.getSignal());
            break;
        case ClickEncoder::Held:
            // Serial.println("Button: HELD");
            if (frequency != lastFrequency && !startedScanning){
                canSetFrequency = false;
                Serial.println("Should scan!");
                startedScanning = true;
                if (frequency > lastFrequency){
                    Serial.println("Scan Up!");
                    radio.scan(1);
                }
                else{
                    Serial.println("Scan Down!");
                    radio.scan(0);
                }
                delay(1000);
            }
            break;
        case ClickEncoder::Released:
            startedScanning = false;
            radio.readData();
            frequency = lastFrequency = radio.getFrequency();
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
    }
}

void cycleMode(){
    switch (currentMode) {
        case Standby:
            currentMode = Radio;
            // turn amplifier pn
            radio.unstandby();// turn radio on
            break;
        case Radio:
            currentMode = Bluetooth;
            radio.standby();
            digitalWrite(BLUETOOTH_FET, ON);
            // turn bluetooth on
            digitalWrite(BLUETOOTH_FET, HIGH);
            break;
        case Bluetooth:
            currentMode = Aux;
            // turn bluetooth off
            digitalWrite(BLUETOOTH_FET, LOW);
            break;
        case Aux:
            currentMode = Standby;
            // turn amplifier off
            digitalWrite(AMP_FET, ON);
            break;
    }
}