// CustomHexTester.ino

void setup() {
	Wire.begin();
	Serial.begin(9600)
}

void loop() {
	Serial.println("How many Colors?");
	colors = Serial.read();
	
}

