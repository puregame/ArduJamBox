// TEA5767 controller class


#include "tea5767.h"
#include <util/twi.h>
#include <Arduino.h>
#include <wire.h>

TEA5767::TEA5767(){
	TEA5767_buffer[0] = 0x00;
	TEA5767_buffer[1] = 0x00;
	TEA5767_buffer[2] = 0xB0;
	TEA5767_buffer[3] = 0x10;
	TEA5767_buffer[4] = 0x00;
}

void TEA5767::standby(){
	TEA5767_buffer[3] |= TEA5767_WBYTE4_STANDBY; // turn standby bit on and nothing else
	WriteData();
}

void TEA5767::unstandby(){
	TEA5767_buffer[3] &= !TEA5767_WBYTE4_STANDBY; //turn only standby bit off
	WriteData();
}

void TEA5767::SetFrequency(int infreq){
	frequency = infreq;
	unsigned int frequencyB  = 4 * (frequency * 10000 + 225000) / 32768;
	TEA5767_buffer[0] = frequencyB >> 8 & (TEA5767_buffer[0] | 0x3F); 
	// this should conserve the mute and Search mode settings
	TEA5767_buffer[1] = frequencyB & 0xFF;
	WriteData();
}
char TEA5767::ReadData(){
	Wire.requestFrom(0x60, 5);
	if (Wire.available()){
		for(int i = 0; i <DATA_SIZE; i++){
			fromRadioChip[i] = Wire.read();
		}
		stereo = (fromRadioChip[2] & TEA5767_RBYTE4_ADCLEVELOUTPUTMASK)>>7;
		signal_level = (fromRadioChip[3] & TEA5767_RBYTE4_ADCLEVELOUTPUTMASK)>>4;
		return 1;
	}
	return 0;
}
void TEA5767::WriteData(){
	digitalWrite(13, 1);
	Wire.beginTransmission(0x60);
	for(char i = 0; i < DATA_SIZE; i++)
		Wire.write(TEA5767_buffer[i]);
	Wire.endTransmission();
}
