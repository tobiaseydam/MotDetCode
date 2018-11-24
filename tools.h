#ifndef TOOLS_H
    #define TOOLS_H

    #include <ArduinoJson.h>
    #include <WString.h>
    #include <SPIFFS.h>
    #include <FS.h>
    #include "debug.h"

    class tools{
        public:
            static void init();
            static void saveJsonFile(String filename, JsonObject *json);
            static JsonObject& loadJsonFile(String filename);
            static bool fileExist(String filename);
            static IPAddress strToIP(String str);
    };

#endif