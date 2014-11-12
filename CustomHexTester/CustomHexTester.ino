#include <Wire.h>

#define ISRTEST

void setup() {
	Wire.begin();
	Serial.begin(115200);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);

    #ifdef ISRTEST
        cli(); // disable global interrupts 
    TCCR2B = 0; // reset control regesters
    TCCR2A = 0;

    OCR2A = 0xFF; // will count all the way up to 255 before tripping the interrupt
    TCCR2B |= (1 << WGM22)
    TCCR2B |= (1 << CS22); // timer2 1024 prescalar
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS20);
    TIMSK2 |= (1 << OCIE2A); // enable output compare interrupts on 2A
    sei(); // enable global interrupts
    #endif
}

#ifdef ISRTEST
isr(TIMER2_COMPA_vect){ // should happen at ~60 hz (16000000 /1024/256) [clock / prescaler / count]
    count = count>60?0: count+1;
    if (count == 0)
        LED_SWITCH; // switch the LED every second
        Serial.println("second");

    if (radio.isScanning && count % 10){ // if the radio is scanning, check for end of band and/or found station 6 times per second
        ledArray.updateRotation();
        if(radio.checkEnd()) { // if the end of the band has been reached
            radio.restartScan(); // restart the scan (from top or bottom, TBD by function)
        }
        else if (radio.ready){ // if the station has been found (just updated data from the if statement)
            radio.stopScanning();
        }
    }
}
#endif

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