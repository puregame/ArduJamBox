
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

void RGBHex::updateRotation(){ // update rotation or blink

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
    }
    writeData();
}