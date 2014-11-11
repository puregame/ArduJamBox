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
	{0,0,0},
};

byte red[3] = {0xFF,0x0,0x0}
byte green[3] = {0x0,0xFF,0x0}
byte blue[3] = {0x0,0x0,0xFF}

byte * currentRGB = RGB_Panel;
void setup()
{
	Wire.begin();
}

void loop()
{
	setSolidColor(red);
	delay(2000);
	setSolidColor(green);
	delay(2000);
	setSolidColor(blue);
	delay(2000);
}

void setSolidColor(byte* color){
	Wire.beginTransmission(1);
	Wire.write(0)
	for (byte i = 0; i < 6; i++){
		Wire.write(color[0]);
		Wire.write(color[1]);
		Wire.write(color[2]);
	}
	Wire.endTransmission();

	Wire.beginTransmission(1);
	Wire.write(18)
	for (byte i = 0; i < 6; i++){
		Wire.write(color[0]);
		Wire.write(color[1]);
		Wire.write(color[2]);
	}
	Wire.endTransmission()

}

void setRGB(){
	Wire.beginTransmission(1);
	Wire.write(0)
	for (byte i = 0; i < 6; i++){
		for (byte j = 0; j < 3; j++){
			Wire.write(currentRGB[i][j]);
		}
	}
	Wire.endTransmission();

	Wire.beginTransmission(1);
	Wire.write(18)
	for (byte i = 6; i < 12; i++){
		for (byte j = 0; j < 3; j++){
			Wire.write(currentRGB[i][j]);
		}
	}
	Wire.endTransmission();
}