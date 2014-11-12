#include <Wire.h>


byte RGB_Panel[12][3] = {
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0}
};

byte red[3] = {0xFF,0x0,0x0};
byte green[3] = {0x0,0xFF,0x0};
byte blue[3] = {0x0,0x0,0xFF};

byte currentRGB[] = {0,0,0};
void setup()
{
	currentRGB[0] = 255;
	currentRGB[1] = 100;
	currentRGB[2] = 0;
	Serial.begin(9600);
	Wire.begin();
}

void loop()
{
	// currentRGB[2] +=20;
	setSolidColor(currentRGB);
	delay(2000);
	printSolidColor(currentRGB);
}

void printSolidColor(byte color[3]){
	Serial.print("Red: ");
	Serial.print(color[0]);
	Serial.print(" Green: ");
	Serial.print(color[1]);
	Serial.print(" Blue: ");
	Serial.println(color[2]);
}

void setSolidColor(byte color[3]){
	Wire.beginTransmission(1);
	Wire.write(0);
	for (byte i = 0; i < 6; i++){
		Wire.write(color[0]);
		Wire.write(color[1]);
		Wire.write(color[2]);
	}
	Wire.endTransmission();

	Wire.beginTransmission(1);
	Wire.write(18);
	for (byte i = 0; i < 6; i++){
		Wire.write(color[0]);
		Wire.write(color[1]);
		Wire.write(color[2]);
	}
	Wire.endTransmission();

}

void setRGB(){
	Wire.beginTransmission(1);
	Wire.write(0);
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 3; j++){
			Wire.write(currentRGB[j]);
		}
	}
	Wire.endTransmission();

	Wire.beginTransmission(1);
	Wire.write(18);
	for (int i = 6; i < 12; i++){
		for (int j = 0; j < 3; j++){
			Wire.write(currentRGB[j]);
		}
	}
	Wire.endTransmission();
}
