#pragma once
#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class GetDataFromJson
{
public:
        static String getDataOneFromJsonStringReturned(String p_valeurRechercher);
};