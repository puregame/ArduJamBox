// customRGBHex.ino.ino

#include <Wire.h>
#define DEBUG 1

#define I2C_ADDRESS = 1

#define PORTB_LED_MASK B00110001
#define PORTD_LED_MASK B10001100

#define ENABLE_ALL_LEDS PORTB |= PORTB_LED_MASK; PORTD |= PORTD_LED_MASK;
#define DISABLE_ALL_LEDS PORTB &= !PORTB_LED_MASK; PORTD &= !PORTD_LED_MASK;

int b1 = 3;
int r1 = 5;
int g1 = 6;
int b2 = 9;
int r2 = 10;
int g2 = 11;

int d1d2 = 2;    //PD2
int d3d4 = 4;    //PD3
int d5d6 = 7;    //PD7
int d7d8 = 8;    //PB0
int d9d10 = 12;  //PB4
int d11d12 = 13; //PB5

int t=1;
int displayControl=1;
int availableBytes=0;

byte colors = 0;
byte ledconfig = 0;
byte rotation =  0;

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

void setup() {
	#if DEBUG
	Serial.begin(115200);
	#endif
	Wire.begin(I2C_ADDRESS)
	Wire.onReceive(receiveEvent);
	initPins();
}



void loop() {
	#if DEBUG
	Serial.prin("Colors: ");
	Serial.print(colors);
	Serial.prin("Config: ");
	Serial.print(ledconfig);
	#endif
	switch (colors) {
	    case 0:
	      oneColor();
	      break;
	    case 1:
	      twoColor();
	      break;
	    case 2:
	      threeColor();
	      break;
	    case 3:
	      // do something
	      break;
	}
}

void oneColor(){
	switch (ledconfig){
		case 0: // single LED is on
			writeSingleLED();
		case 1: // ALL leds are on
			ENABLE_ALL_LEDS
		    analogWrite(r1,RGB[0][0]);
		    analogWrite(g1,RGB[0][1]);
		    analogWrite(b1,RGB[0][2]);
		    analogWrite(r2,RGB[0][0]);
		    analogWrite(g2,RGB[0][1]);
		    analogWrite(b2,RGB[0][2]);
		    break;
		case 2: // top two LEDs are on
			DISABLE_ALL_LEDS
			digitalWrite(d1d2, HIGH);
			analogWrite(r1,RGB[0][0]);
		    analogWrite(g1,RGB[0][1]);
		    analogWrite(b1,RGB[0][2]);
		    analogWrite(r2,RGB[0][0]);
		    analogWrite(g2,RGB[0][1]);
		    analogWrite(b2,RGB[0][2]);
		case 3: // bottom two LEDs are on
			DISABLE_ALL_LEDS
			digitalWrite(d7d8, HIGH);
			analogWrite(r1,RGB[0][0]);
		    analogWrite(g1,RGB[0][1]);
		    analogWrite(b1,RGB[0][2]);
		    analogWrite(r2,RGB[0][0]);
		    analogWrite(g2,RGB[0][1]);
		    analogWrite(b2,RGB[0][2]);
	}
}

void writeSingleLED(){
	// turn everything off
	DISABLE_ALL_LEDS
    analogWrite(r1,LOW);
    analogWrite(g1,LOW);
    analogWrite(b1,LOW);
    analogWrite(r2,LOW);
    analogWrite(g2,LOW);
    analogWrite(b2,LOW);
    switch (rotationPos) {
	    case 0:
			digitalWrite(d1d2, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			break;
        case 1:
			digitalWrite(d1d2, HIGH);
			analogWrite(r2, RGB[0][0]);
			analogWrite(g2, RGB[0][1]);
			analogWrite(b2, RGB[0][2]);
			break;
		case 2;
			digitalWrite(d3d4, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			break;
		case 3:
			digitalWrite(d3d4, HIGH);
			analogWrite(r2, RGB[0][0]);
			analogWrite(g2, RGB[0][1]);
			analogWrite(b2, RGB[0][2]);
			break;
		case 4:
			digitalWrite(d5d6, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			break;
		case 5:
			digitalWrite(d5d6, HIGH);
			analogWrite(r2, RGB[0][0]);
			analogWrite(g2, RGB[0][1]);
			analogWrite(b2, RGB[0][2]);
			break;
		case 6:
			digitalWrite(d7d8, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			break;
		case 7:
			digitalWrite(d7d8, HIGH);
			analogWrite(r2, RGB[0][0]);
			analogWrite(g2, RGB[0][1]);
			analogWrite(b2, RGB[0][2]);
			break;
		case 8:
			digitalWrite(d9d10, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			break;
		case 9:
			digitalWrite(d9d10, HIGH);
			analogWrite(r2, RGB[0][0]);
			analogWrite(g2, RGB[0][1]);
			analogWrite(b2, RGB[0][2]);
			break;
		case 10:
			digitalWrite(d11d12, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			break;
		case 11:
			digitalWrite(d11d12, HIGH);
			analogWrite(r2, RGB[0][0]);
			analogWrite(g2, RGB[0][1]);
			analogWrite(b2, RGB[0][2]);
			break;
		default:
			break;
    }

}

void twoColor(){
	DISABLE_ALL_LEDS
	switch (ledconfig) {
		case 0:
			digitalWrite(d1d2, HIGH);
			digitalWrite(d3d4, HIGH);
			digitalWrite(d5d6, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			analogWrite(r1, RGB[1][0]);
			analogWrite(g1, RGB[1][1]);
			analogWrite(b1, RGB[1][2]);
			DISABLE_ALL_LEDS
			digitalWrite(d7d8, HIGH);
			digitalWrite(d9d10, HIGH);
			digitalWrite(d11d12, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			analogWrite(r1, RGB[1][0]);
			analogWrite(g1, RGB[1][1]);
			analogWrite(b1, RGB[1][2]);
			break;
		case 1:
			digitalWrite(d1d2, HIGH);
			digitalWrite(d9d10, HIGH);
			digitalWrite(d5d6, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			analogWrite(r1, RGB[1][0]);
			analogWrite(g1, RGB[1][1]);
			analogWrite(b1, RGB[1][2]);
			DISABLE_ALL_LEDS
			digitalWrite(d3d4, HIGH);
			digitalWrite(d7d8, HIGH);
			digitalWrite(d11d12, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			analogWrite(r1, RGB[1][0]);
			analogWrite(g1, RGB[1][1]);
			analogWrite(b1, RGB[1][2]);
			break;
		case 2:
			ENABLE_ALL_LEDS
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			analogWrite(r1, RGB[1][0]);
			analogWrite(g1, RGB[1][1]);
			analogWrite(b1, RGB[1][2]);
			break;
		case 3:
			digitalWrite(d1d2, HIGH);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			analogWrite(r1, RGB[0][0]);
			analogWrite(g1, RGB[0][1]);
			analogWrite(b1, RGB[0][2]);
			DISABLE_ALL_LEDS
			digitalWrite(d7d8, HIGH);
			analogWrite(r1, RGB[1][0]);
			analogWrite(g1, RGB[1][1]);
			analogWrite(b1, RGB[1][2]);
			analogWrite(r1, RGB[1][0]);
			analogWrite(g1, RGB[1][1]);
			analogWrite(b1, RGB[1][2]);
			break;
		default:
			// do something
	}
}

void threeColor(){

}

void receiveEvent(int howMany)
{
	int data = Wire.read();
	colors   = (data >> 6) & 0x3;
	ledconfig= (data >> 4) & 0x3;
	rotation = (data     ) & 0xF;
	int beginPos=Wire.read();
	availableBytes=Wire.available();
	for(int i=beginPos; i<beginPos+availableBytes; i++)
		RGB[i/3][i%3]=Wire.read();
}

void scan(){
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

void initPins(){
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

