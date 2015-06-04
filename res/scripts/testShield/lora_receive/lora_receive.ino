/**************************************************************************/
/*!
    @file     lora_receive.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

	Receive messages using LoRa Shield from Froggy Factory - Wi6labs
	
    @section  HISTORY

    v0.1 - First beta release
*/
/**************************************************************************/


#include <SPI.h>
#include <FroggyFactory_Lora.h>
#include <Wire.h>
#include <Deuligne.h>

#define LOOP_DELAY 100
#define CSPIN 10
#define FREQ 868300000


FROGGYFACTORY_LORA myLoRaShield;
//FROGGYFACTORY_LORA myLoRaShield(CSPIN, FREQ); //alternative constructor call

int available_payload;

// initialize the library with the numbers of the interface pins
Deuligne lcd;


void setup() {
	
	myLoRaShield.lora_init();
	
	// Alternative init call
	//myLoRaShield.lora_init(false);
	//myLoRaShield.lora_rfConf(3);
	//myLoRaShield.lora_setChannelFreq(255); // 255 = default channel
        
        //lcd Init
        lcd.init();	

	//Start serial always after LoRa Init
	Serial.begin(9600);
	while (!Serial) ; //wait until Serial ready
	
	Serial.println("Startup of LoRa Receiver...");
        lcd.print("Start Receiver");
        lcd.setCursor(0, 0);
}

void loop() {

	available_payload = myLoRaShield.lora_available();
	
	if (available_payload) {
		Serial.print("Bytes received: ");
		Serial.println(available_payload, DEC);
		Serial.print("Payload: ");
		
                char pl_arr[available_payload];
		for (int i=0; i < available_payload; i++) {
                        char pl = myLoRaShield.lora_read();
                        pl_arr[i] = pl;
			Serial.print(pl);
		}
                String str(pl_arr);
                lcd.print(str);
                lcd.setCursor(0, 0);
		Serial.println("   ---PAYLOAD END");
	}
	
	Serial.print("SNR: ");
	Serial.print (myLoRaShield.lora_getSnr());
	Serial.print(" | RSSI: ");
	Serial.println (myLoRaShield.lora_getRssi());
	
	delay(LOOP_DELAY);
}

