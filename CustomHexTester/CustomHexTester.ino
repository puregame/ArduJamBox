#include <Wire.h>

void setup() {
	Wire.begin();
	Serial.begin(115200);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
}

unsigned char colorsArray[4][3] = {{0,0,0},{0,0,0},{0,0,0}};
char toTransmit = 0;
char colors = 0;

void loop() {
	Serial.println("How many Colors?");
    while(Serial.available() == 0){}
	colors = char(Serial.parseInt());
    Serial.print("Will ask for: ");
    Serial.print(int(colors));
	Serial.print("colors");
    for(int i=0; i<colors; i++){
        Serial.print("Red value: ");
        while(Serial.available() == 0){}
        colorsArray[i][0] = char(Serial.parseInt());
        Serial.print("Green value: ");
        while(Serial.available() == 0){}
        colorsArray[i][1] = char(Serial.parseInt());
        Serial.print("Blue value: ");
        while(Serial.available() == 0){}
        colorsArray[i][2] = char(Serial.parseInt());
    }
    Serial.println("What is the config code? ");
    while(Serial.available() == 0){}
    char ledconfig = char(Serial.parseInt());
    char ledToLight = 0;
    if(colors == 1 && ledconfig == 0){
        Serial.println("What is the LED that you want litt up?");
        while(Serial.available() == 0){}
        ledToLight = char(Serial.parseInt());
    }

    toTransmit = ((colors << 6) | (ledconfig << 4) | ledToLight);
    writeData();
}

void writeData(){
    Wire.beginTransmission(1);
    Wire.write(toTransmit);
    for(int i=0; i<colors; i++){
        Wire.write(colorsArray[i][0]);
        Wire.write(colorsArray[i][1]);
        Wire.write(colorsArray[i][2]);
    }
    Wire.endTransmission();
}