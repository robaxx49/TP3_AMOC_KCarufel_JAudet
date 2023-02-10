#include <Arduino.h>
#include "GetDataFromJson.h"


    
    String GetDataFromJson::getDataOneFromJsonStringReturned(String p_valeurRechercher)
    {
         File file = SPIFFS.open("/settings.json","r");
         DynamicJsonDocument doc(1024);
         DeserializationError err = deserializeJson(doc,file);
         if (!err)
         {
            return doc[p_valeurRechercher].as<String>();
         }
         else
         {
            return "";
         }
         
    }
