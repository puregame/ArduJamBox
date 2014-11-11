// CustomHexTester.ino

void setup() {
	Wire.begin();
	Serial.begin(9600)
}

unsigned char * colorsArray;
char toTransmit = 0;
char colors = 0;

void loop() {
	Serial.println("How many Colors?");
	colors = char(Serial.parseInt());
    Serial.print("Will ask for: ");
    Serial.print(colors);
	Serial.print("colors");
    colorsArray = new unsigned char[colors][3];
    for(int i=0; i<colors; i++){
        Serial.print("Red value: ");
        colorsArray[i][0] = char(Serial.parseInt());
        Serial.print("Green value: ");
        colorsArray[i][1] = char(Serial.parseInt());
        Serial.print("Blue value: ");
        colorsArray[i][2] = char(Serial.parseInt());
    }
    Serial.println("What is the config code? ");
    char ledconfig = char(Serial.parseInt());
    char ledToLight = 0;
    if(colors == 1 && config == 0){
        Serial.prinln("What is the LED that you want litt up?")
        ledToLight = char(Serial.parseInt());
    }

    toTransmit = ((colors << 6) | (ledconfig << 4) | ledToLight);

}

void writeData(){
    Wire.beginTransmission(1);
    Wire.write(toTransmit);
    for(int i=0; i<colors; i++){
        Wire.write(colorsArray[i][0]);
        Wire.write(colorsArray[i][1]);
        Wire.write(colorsArray[i][2]);
    }
    wire.endTransmission();
}