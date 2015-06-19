#include <SPI.h>
#include <FroggyFactory_Lora.h>

#define LOOP_DELAY 5000
#define CSPIN 10
#define FREQ 868300000

#define LED 7


FROGGYFACTORY_LORA loraShield;

void show_infos()
{
   Serial.print("RF : ");
   Serial.println(loraShield.lora_getRFConf());
   delay(500);
   Serial.print("SNR : ");
   Serial.println(loraShield.lora_getSnr());
   delay(500);
   Serial.print("RSSI : ");
   Serial.println(loraShield.lora_getRssi());
   delay(500);
   Serial.print("Freq : ");
   Serial.println(loraShield.lora_getFreq()); 
}

void show_menu()
{
  Serial.println("Commands :");
  Serial.println("\"menu\" - Affiche ce menu");
  Serial.println("\"affichage\" - Affiche les informations du shield");
  Serial.println("\"rfx\" ou x est un nombre entre 0 et 3 - Change le facteur rf");
  Serial.println("\"chanx\" ou x est un nombre entre 0 et 7 - Change le shield de channel"); 
  Serial.println("\"msg:\" suivi du message a envoyer - Change le shield de channel"); 
}

void setup() {
	
	loraShield.lora_init();
	
	//Start serial always after LoRa Init
	Serial.begin(9600);
	while (!Serial) ; //wait until Serial ready
	
	Serial.println("Startup of LoRa transmitter...");

        show_menu();

        pinMode(LED, OUTPUT);
}

void loop() {
  
    String content = "";
    char character;
  
    while(Serial.available()) {
        character = Serial.read();
        content.concat(character);
        delay(10);
    }
  
    if (content == "menu")
      show_menu();
    else if(content == "affichage")
      show_infos();
    else if(content.substring(0,2) == "rf")
    {
      int rf = content.substring(2).toInt();
      if(rf <= 3 && rf >= 0) {
        loraShield.lora_rfConf(rf);
        Serial.println("Rf Modifie");
      }
      else
        Serial.println("Modification echouee");
      Serial.println(rf);
    }
    else if(content.substring(0,4) == "chan")
    {
      int fr = content.substring(4).toInt();
      if(fr <= 7 && fr >= 0) {
        loraShield.lora_setChannelFreq(fr);
        Serial.println("Channel modifie");
      }
      else
        Serial.println("Modification echouee");
    }
    else if(content.substring(0,4) == "msg:")
    {
      String msg = content.substring(4);
      loraShield.lora_send("MSG : "+msg); 
      Serial.println(msg);
    }
    else if(content != "")
      Serial.println("Commande non reconnue : " + content);
      
    //Check Message
    int available_payload = loraShield.lora_available();
    if (available_payload) {
      char pl_arr[available_payload];
      for (int i=0; i < available_payload; i++) {
        char pl = loraShield.lora_read();
        pl_arr[i] = pl;
	Serial.print(pl);
       }
    }
}

