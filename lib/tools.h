#ifndef TOOLS_H
    #define TOOLS_H

    #include <ArduinoJson.h>
    #include <WString.h>
    #include <SPIFFS.h>
    #include <FS.h>

    class tools{
        public:
            static void init();
            static void saveJsonFile(String filename, JsonObject *json);
            static JsonObject& loadJsonFile(String filename);
            static bool fileExist(String filename);
    };

#endif