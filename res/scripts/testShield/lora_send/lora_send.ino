/**************************************************************************/
/*!
    @file     lora_receive.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

	Transmit messages using LoRa Shield from Froggy Factory - Wi6labs
	
    @section  HISTORY

    v0.1 - First beta release
*/
/**************************************************************************/


#include <SPI.h>
#include <FroggyFactory_Lora.h>

#define LOOP_DELAY 5000
#define CSPIN 10
#define FREQ 868300000

#define LED 7


FROGGYFACTORY_LORA myLoRaShield;
//FROGGYFACTORY_LORA myLoRaShield(CSPIN, FREQ); //alternative constructor call

String payload_ON = "LED ALLUMEE !";
String payload_OFF = "LED ETEINTE ! ";
String payload;
boolean allume = true;
unsigned long start;


void setup() {
	
	myLoRaShield.lora_init();
	
	// Alternative init call
	//myLoRaShield.lora_init(false);
	//myLoRaShield.lora_rfConf(3);
	//myLoRaShield.lora_setChannelFreq(255); // 255 = default channel
	
	//Start serial always after LoRa Init
	Serial.begin(9600);
	while (!Serial) ; //wait until Serial ready
	
	Serial.println("Startup of LoRa transmitter...");

        pinMode(LED, OUTPUT);
        start = millis();
}

void loop() {
  char carlu = 0;
  int cardispo = 0;
  
  cardispo = Serial.available();
    
  while(cardispo > 0) 
  {
    carlu = Serial.read();
    if(carlu == 'a')
    {
      payload = payload_ON;
      digitalWrite(LED, HIGH);
      Serial.println("Allumage de la LED");
    }
    if(carlu == 'e')
    {
      payload = payload_OFF;
      digitalWrite(LED, LOW);
      Serial.println("Extinction de la LED");
    }
    cardispo = Serial.available();
    myLoRaShield.lora_send(payload);
    delay(2000);
  }
}

