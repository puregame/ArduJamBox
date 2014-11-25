#include "ClickEncoder.h"
#include "TEA5767.h"
#include "DIY_Radio.h"
#include <Wire.h>
#include <EEPROM.h>
#include "TimerOne.h"

// Custom classes and objects
TEA5767 radio;
// RGBHex  ledArray;
Mode    currentMode = Radio;

// objecs from library
ClickEncoder *encoder;

// Radio variables
int16_t lastFrequency,frequency = 9532;
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
     //pinmode settings
    pinMode(ledPin, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(BLUETOOTH_FET, OUTPUT);
    digitalWrite(BLUETOOTH_FET, LOW);
    pinMode(SPEAKER_FET, OUTPUT);
    pinMode(RADIO_FET, OUTPUT);
    pinMode(AMP_FET, OUTPUT);

    Serial.begin(115200);
    encoder = new ClickEncoder(A1, A0, A2);
    Wire.begin();
    Timer1.initialize(1000); // run timerIsr every 1ms
    Timer1.attachInterrupt(timerIsr); 
    readFreq();
   

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
                writeFreq();
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
            // encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
            // Serial.print("  Acceleration is ");
            // Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
            cycleMode();
            break;
    }
}

void cycleMode(){
    writeMode();
    switch (currentMode) {
        // case Standby:
        //     currentMode = Radio;
        //     // turn amplifier pn
        //     radio.unstandby();// turn radio on
        //     break;
        case Radio:
            currentMode = Bluetooth;
            Serial.println("Changed to bluetooth");
            radio.standby();
            BLUETOOTH_ON 
            // turn bluetooth on
            break;
        case Bluetooth:
            currentMode = Radio;
            Serial.println("Changed to radio");
            radio.unstandby();
            // turn bluetooth off
            BLUETOOTH_OFF
            break;
        // case Aux:
        //     currentMode = Standby;
        //     // turn amplifier off
        //     digitalWrite(AMP_FET, ON);
        //     break;
    }
}

void writeFreq(){
    EEPROM.write(0, frequency & 0xFF); // write the low side fo the number
    EEPROM.write(1, frequency>>8);     // write the high side of the number
}

void readFreq(){
    frequency = (EEPROM.read(1) << 8) | (EEPROM.read(0) & 0xFF);  // read and parse the number
}

void writeMode(){
    switch (currentMode) {
        case Radio:
          EEPROM.write(3, 0);
          break;
        case Bluetooth:
          EEPROM.write(3,1);
          break;
    }
    EEPROM.write(3, currentMode);
}

void readMode(){
    byte input = EEPROM.read(3);
    switch (input) {
        case 1:
          currentMode = Radio;
          break;
      case 0:
          currentMode = Bluetooth;
          break;
    }
}