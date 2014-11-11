

#include "tea5767.h"
#include <util/twi.h>
#include <Arduino.h>
#include <wire.h>

RGBHex::RGBHex(){
    isRotating = 0;
    RotationDirection = none;
    memset(nullColors, 0, 12)
}

RGBHex::writeData(){
    char toTransmit = ((colors << 6) | (ledconfig << 4) | rotation);
    Wire.beginTransmission(1);
    Wire.write(toTransmit);
    for(int i=0; i<colors; i++){
        Wire.write(currentColorsArray[i][0]);
        Wire.write(currentColorsArray[i][1]);
        Wire.write(currentColorsArray[i][2]);
    }
    wire.endTransmission();
}

void RGBHex::setPrimaryColor(char r, char g, char b){
    colorsArray[0][0] = r;
    colorsArray[0][1] = g;
    colorsArray[0][2] = b;
}
void RGBHex::setSecondaryColor(char r, char g, char b){
    colorsArray[1][0] = r;
    colorsArray[1][1] = g;
    colorsArray[1][2] = b;
}
void RGBHex::setupTimer2(){ /// 
    // setup timer2 for interrupt to blink or rotate
    cli(); // disable global interrupts 
    TCCR2B = 0; // reset control regesters
    TCCR2A = 0;

    OCR2A = 0xFF;
    TCCR2B |= (1 << WGM22)
    TCCR2B |= (1 << CS22); // timer2 1024 prescalar
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS20);
    TIMSK2 |= (1 << OCIE2A);
    sei(); // enable global interrupts
}
/* // the following function should be placed in the mian .ino file to run the updateRotation fn every time the vector goes off
isr(TIMER2_COMPA_vect){
  ledArray.updateRotation();
}
*/
void RGBHex::updateRotation(){

    if (RotationDirection == CW){
        rotation--;
    }
    else if (RotationDirection == CCW){
        rotation++;
    }
    else{
        if (isBlinking){
            if(ison){
                ison = false;
                currentColorsArray = nullColors;
            }
            else{
                ison = true;
                currentColorsArray = colorsArray;
            }
        }
        else{
            currentColorsArray = colorsArray;
        }
        // check blinking
    }
    writeData();
}