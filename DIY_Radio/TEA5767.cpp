// TEA5767 controller class


#include "tea5767.h"
#include <util/twi.h>
#include <Arduino.h>
#include <wire.h>

TEA5767::TEA5767(){
	isScanning = false;
	TEA5767_buffer[0] = 0x00;
	TEA5767_buffer[1] = 0x00;
	TEA5767_buffer[2] = 0xB0;
	TEA5767_buffer[3] = 0x10;
	TEA5767_buffer[4] = 0x00;
}

void TEA5767::standby(){
	TEA5767_buffer[3] |= TEA5767_WBYTE4_STANDBY; // turn standby bit on and nothing else
	writeData();
}

void TEA5767::unstandby(){
	TEA5767_buffer[3] &= !TEA5767_WBYTE4_STANDBY; //turn only standby bit off
	writeData();
}

void TEA5767::scan(bool direction){
	scanDir = direction;
	readData();
	TEA5767_buffer[0] = TEA5767_WBYTE1_SEARCHMODE | (fromRadioChip[0] & 0x3F); // combine serachmode bit and current upper pll bits
	TEA5767_buffer[1] = fromRadioChip[1];
	// set search level to mid, direction based on the input direction (0-down, 1-up), stoplevel mid, set HLSI big
	TEA5767_buffer[2] = (scanDir << 7) | TEA5767_WBYTE3_SEARCHSTOPLEVEL_HIGH | TEA5767_WBYTE3_HLSI;
	TEA5767_buffer[3] = TEA5767_buffer[3]; // don't change this big, nothing special to cheange
	TEA5767_buffer[4] = 0x00;
	writeData();
}

void TEA5767::setFrequency(int infreq){
	frequency = infreq;
	unsigned int frequencyB  = 4 * (frequency * 10000 + 225000) / 32768;
	TEA5767_buffer[0] = frequencyB >> 8 & (TEA5767_buffer[0] | 0x3F); 
	// this should conserve the mute and Search mode settings
	TEA5767_buffer[1] = frequencyB & 0xFF;
	writeData();
}
void TEA5767::readData(){
	Wire.requestFrom(0x60, 5);
	unsigned long freqA;
	if (Wire.available()){
		for(int i = 0; i <DATA_SIZE; i++){
			fromRadioChip[i] = Wire.read();
		}
		stereo 		= (fromRadioChip[2] & TEA5767_RBYTE4_ADCLEVELOUTPUTMASK) >> 7;
		signal_level= (fromRadioChip[3] & TEA5767_RBYTE4_ADCLEVELOUTPUTMASK) >> 4;
		bandLimit 	= (fromRadioChip[0] & TEA5767_RBYTE1_BANDLIMITFLAG) 	 >> 6;
		ready 		= (fromRadioChip[0] & TEA5767_RBYTE1_READYFLAG) 		 >> 7;

		freqA = (fromRadioChip[0] & 0x3F) <<8 | (fromRadioChip[1]);
		frequency = ((freqA*8192)-225000) / 10000;
	}
	Serial.print("New frequency from device: ");
	Serial.println(freqA);
	Serial.print("New frequency efrom devic: ");
	Serial.println(frequency);
	Serial.print("ready: ");
	Serial.println(ready);
}
void TEA5767::writeData(){
	digitalWrite(13, 1);
	Wire.beginTransmission(0x60);
	for(char i = 0; i < DATA_SIZE; i++)
		Wire.write(TEA5767_buffer[i]);
	Wire.endTransmission();
}

bool TEA5767::checkEnd(){
	readData();
	return bandLimit;
}

void TEA5767::restartScan(){
	// default to top of band
	int startFeq = TEA5767_FM_BAND_START;
	if (scanDir){
		startFeq = TEA5767_FM_BAND_END;
	}
	unsigned int frequencyB  = 4 * (startFeq * 10000 + 225000) / 32768;
	readData();
	TEA5767_buffer[0] = TEA5767_WBYTE1_SEARCHMODE | (frequencyB >> 8 & (TEA5767_buffer[0] | 0x3F)) ; // combine serachmode bit and current upper pll bits
	TEA5767_buffer[1] = frequencyB & 0xFF;
	// set search level to mid, direction based on the input direction (0-down, 1-up), stoplevel mid, set HLSI big
	TEA5767_buffer[2] = (scanDir << 7) | TEA5767_WBYTE3_SEARCHSTOPLEVEL_MID | TEA5767_WBYTE3_HLSI;
	TEA5767_buffer[3] = TEA5767_buffer[3]; // don't change this big, nothing special to cheange
	TEA5767_buffer[4] = 0x00;
	writeData();
}