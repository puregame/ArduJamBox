//  Puzzlebox RGB HEX board Firmware 
//  This firmware is developed for Puzzlebox's RGB HEX
//  Choose Duemilanove w/ATmega328 when program this RGB HEX.
//  This code ontrols all lights on RGB HEX, and can be updated
//  via I2C/TWI communication.
//  Author: Azureviolin <www.azureviolin.com>
//  Copyright (C) 2013 Puzzlebox Productions LLC
// 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include <Wire.h>
#define DEBUG 1

int b1 = 3;
int r1 = 5;
int g1 = 6;
int b2 = 9;
int r2 = 10;
int g2 = 11;

int d1d2 = 2;
int d3d4 = 4;
int d5d6 = 7;
int d7d8 = 8;
int d9d10 = 12;
int d11d12 = 13;

int t=1;
int displayControl=1;
int availableBytes=0;

byte RGB[12][3]={
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
    {255,255,255},
  };


void setup()
{
  #if DEBUG
  Serial.begin(115200);
  #endif
  Wire.begin(1);                // join i2c bus with address #1
  Wire.onReceive(receiveEvent); // register event
  initPins();

}

void loop()
{
  scan();//scan the display
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  int beginPos=Wire.read();
  availableBytes=Wire.available();
  for(int i=beginPos; i<beginPos+availableBytes; i++)
    RGB[i/3][i%3]=Wire.read();
}

#if DEBUG
long loopStartTime;
#endif

void scan()
{
  for (int i = 0; i < displayControl; i++) {

//    #if DEBUG    
//    loopStartTime=micros();
//    #endif
    
    digitalWrite(d1d2,HIGH);
    analogWrite(r1,RGB[0][0]);
    analogWrite(g1,RGB[0][1]);
    analogWrite(b1,RGB[0][2]);
    analogWrite(r2,RGB[1][0]);
    analogWrite(g2,RGB[1][1]);
    analogWrite(b2,RGB[1][2]);
    analogWrite(r1,LOW);
    analogWrite(g1,LOW);
    analogWrite(b1,LOW);
    analogWrite(r2,LOW);
    analogWrite(g2,LOW);
    analogWrite(b2,LOW);
    digitalWrite(d1d2,LOW);
    
//    #if DEBUG
//    Serial.print("D12:");
//    Serial.println(micros()-loopStartTime);
//    #endif
    
    digitalWrite(d3d4,HIGH);
    analogWrite(r1,RGB[2][0]);
    analogWrite(g1,RGB[2][1]);
    analogWrite(b1,RGB[2][2]);
    analogWrite(r2,RGB[3][0]);
    analogWrite(g2,RGB[3][1]);
    analogWrite(b2,RGB[3][2]);
    analogWrite(r1,LOW);
    analogWrite(g1,LOW);
    analogWrite(b1,LOW);
    analogWrite(r2,LOW);
    analogWrite(g2,LOW);
    analogWrite(b2,LOW);
    digitalWrite(d3d4,LOW);
    
    digitalWrite(d5d6,HIGH);
    analogWrite(r1,RGB[4][0]);
    analogWrite(g1,RGB[4][1]);
    analogWrite(b1,RGB[4][2]);
    analogWrite(r2,RGB[5][0]);
    analogWrite(g2,RGB[5][1]);
    analogWrite(b2,RGB[5][2]);
    analogWrite(r1,LOW);
    analogWrite(g1,LOW);
    analogWrite(b1,LOW);
    analogWrite(r2,LOW);
    analogWrite(g2,LOW);
    analogWrite(b2,LOW);
    digitalWrite(d5d6,LOW);
    
    digitalWrite(d7d8,HIGH);
    analogWrite(r1,RGB[6][0]);
    analogWrite(g1,RGB[6][1]);
    analogWrite(b1,RGB[6][2]);
    analogWrite(r2,RGB[7][0]);
    analogWrite(g2,RGB[7][1]);
    analogWrite(b2,RGB[7][2]);
    analogWrite(r1,LOW);
    analogWrite(g1,LOW);
    analogWrite(b1,LOW);
    analogWrite(r2,LOW);
    analogWrite(g2,LOW);
    analogWrite(b2,LOW);
    digitalWrite(d7d8,LOW);
    
    digitalWrite(d9d10,HIGH);
    analogWrite(r1,RGB[8][0]);
    analogWrite(g1,RGB[8][1]);
    analogWrite(b1,RGB[8][2]);
    analogWrite(r2,RGB[9][0]);
    analogWrite(g2,RGB[9][1]);
    analogWrite(b2,RGB[9][2]);
    analogWrite(r1,LOW);
    analogWrite(g1,LOW);
    analogWrite(b1,LOW);
    analogWrite(r2,LOW);
    analogWrite(g2,LOW);
    analogWrite(b2,LOW);
    digitalWrite(d9d10,LOW);
    
    digitalWrite(d11d12,HIGH);
    analogWrite(r1,RGB[10][0]);
    analogWrite(g1,RGB[10][1]);
    analogWrite(b1,RGB[10][2]);
    analogWrite(r2,RGB[11][0]);
    analogWrite(g2,RGB[11][1]);
    analogWrite(b2,RGB[11][2]);
    analogWrite(r1,LOW);
    analogWrite(g1,LOW);
    analogWrite(b1,LOW);
    analogWrite(r2,LOW);
    analogWrite(g2,LOW);
    analogWrite(b2,LOW);
    digitalWrite(d11d12,LOW);
    
//    #if DEBUG
////    Serial.print("LT:");
//    Serial.println(micros()-loopStartTime);
//    #endif
    //delay(t);
  }
}

void initPins()
{  
  pinMode(b1,OUTPUT);
  pinMode(r1,OUTPUT);
  pinMode(g1,OUTPUT);
  pinMode(b2,OUTPUT);
  pinMode(r2,OUTPUT);
  pinMode(g2,OUTPUT);
  
  pinMode(d1d2,INPUT);
  pinMode(d3d4,INPUT);
  pinMode(d5d6,INPUT);
  pinMode(d7d8,INPUT);
  pinMode(d9d10,INPUT);
  pinMode(d11d12,INPUT);
  
  analogWrite(b1,LOW);
  analogWrite(r1,LOW);
  analogWrite(g1,LOW);
  analogWrite(b2,LOW);
  analogWrite(r2,LOW);
  analogWrite(g2,LOW);
  
  analogWrite(d1d2,LOW);
  analogWrite(d3d4,LOW);
  analogWrite(d5d6,LOW);
  analogWrite(d7d8,LOW);
  analogWrite(d9d10,LOW);
  analogWrite(d11d12,LOW);
}