#include "ClickEncoder.h"
#include "TEA5767.h"
#include <Wire.h>
#include "TimerOne.h"


// Pinout:
  // # A0: Rotory A
  // # A1: Rotory B
  // # A2: Button on swithcer
  // # A4: DTA - Red on pcb
  // # A5: CLK - back on pcb

#define radioFET     5
#define bluetoothFET 6
#define speakerFET   7
#define ledPin       13

TEA5767 radio;
RGBHex  ledArray;

ClickEncoder *encoder;
int16_t last,frequency = 9910;

byte old_mute=1;
byte mute=0;
long loopnum = 0;

byte old_signal_level=1;
byte signal_level=0;

void timerIsr() {
  encoder->service();
}

isr(TIMER2_COMPA_vect){
  ledArray.updateRotation();
}

void setup() {
  Serial.begin(9600);
  encoder = new ClickEncoder(A1, A0, A2);
  Wire.begin();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  pinMode(ledPin, OUTPUT);
  
  last = -1;

 
}

void loop() {  
  // digitalWrite(ledPin, LOW);
  // loopnum +=1;
  // if (loopnum % 10000 && radio.ReadData()){
  //   Serial.println(radio.getStereo());
  //   Serial.println(radio.getSignal());
  //   delay(100);

  // }

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
