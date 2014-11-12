#include "ClickEncoder.h"
#include "TEA5767.h"
#include "DIY_Radio.h"
#include <Wire.h>
#include "TimerOne.h"


// Custom classes and objects
TEA5767 radio;
RGBHex  ledArray;
Mode currentMode;

// objecs from library
ClickEncoder *encoder;

// Radio variables
int16_t last,frequency = 9910;
byte old_mute=1;
byte mute=0;
long loopnum = 0;
byte old_signal_level=1;
byte signal_level=0;

//LED variables
volatile int count = 0;


//Interrupt Service Routines
void timerIsr() {
    encoder->service();
}

isr(TIMER2_COMPA_vect){ // should happen at ~60 hz (16000000 /1024/256) [clock / prescaler / count]
    count = count>60?0: count+1;
    if (count == 0)
        LED_SWITCH; // switch the LED every second

    if (radio.isScanning && count % 10){ // if the radio is scanning, check for end of band and/or found station 6 times per second
        ledArray.updateRotation();
        if(radio.checkEnd()) { // if the end of the band has been reached
            radio.restartScan(); // restart the scan (from top or bottom, TBD by function)
        }
        else if (radio.ready){ // if the station has been found (just updated data from the if statement)
            radio.stopScanning();
        }
    }
}

void setup() {
    Serial.begin(9600);
    encoder = new ClickEncoder(A1, A0, A2);
    Wire.begin();
    Timer1.initialize(1000);
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

    OCR2A = 0xFF; // will count all the way up to 255 before tripping the interrupt
    TCCR2B |= (1 << WGM22)
    TCCR2B |= (1 << CS22); // timer2 1024 prescalar
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS20);
    TIMSK2 |= (1 << OCIE2A); // enable output compare interrupts on 2A
    sei(); // enable global interrupts
}

void loop() {

    frequency += encoder->getValue();
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
    
    ClickEncoder::Button b = encoder->getButton();
    if (b != ClickEncoder::Open) {
        Serial.print("Button: ");
        #define VERBOSECASE(label) case label: Serial.println(#label); break;
        switch (b) {
            case ClickEncoder::Clicked:
                Serial.println("pressed!");
                radio.ReadData();
                Serial.print("Signal Level is: ");
                Serial.println(radio.getSignal());
                break;
            VERBOSECASE(ClickEncoder::Held)
            VERBOSECASE(ClickEncoder::Released)
            case ClickEncoder::DoubleClicked:
                Serial.println("ClickEncoder::DoubleClicked");
                encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
                Serial.print("  Acceleration is ");
                Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
                break;
        }
    }
}

void cycleMode(){
    switch (currentMode) {
        case standby:
            currentMode = radio;
            // turn amplifier pn
            radio.unstandby();// turn radio on
            break;
        case radio:
            currentMode = bluetooth;
            radio.standby();
            // turn bluetooth on
            break;
        case bluetooth:
            currentMode = aux;
            // turn bluetooth off, l
            break;
        case aux:
            currentMode = standby;
            // turn amplifier off
            break;
    }
}

void bluetoothOn(){
    digitalWrite(BLUETOOTH_FET, HIGH);
}
void bluetoothOff(){
    digitalWrite(BLUETOOTH_FET, LOW);
}
