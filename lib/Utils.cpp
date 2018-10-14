#ifndef UTILS_CPP
    #define UTILS_CPP
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "FS.h"
    #include "SPIFFS.h"

    static void serialLog(const char* msg);
    static void serialLogln(const char* msg);

    static void serialLog(const char* msg){
        #ifdef DEBUG
            Serial.print(msg);
        #endif
    }

    static void serialLogln(const char* msg){
        #ifdef DEBUG
            Serial.println(msg);
        #endif
    }

    static void fileSave_WiFi(const char* filename, const char* ssid, const char* pass){
        File file = SPIFFS.open(filename, "w");
        file.print("ssid:");
        file.println(ssid);
        file.print("pass:");
        file.println(pass);
        file.close();       
        
        serialLogln(filename);
        serialLog("ssid:");
        serialLogln(ssid);
        serialLog("pass:");
        serialLogln(pass);
        serialLogln("Datei gespeichert");
    }

    static void fileSave_MQTT(const char* filename, const char* server, const char* user, const char* pass,
        const char* devName){
        File file = SPIFFS.open(filename, "w");
        file.print("server:");
        file.println(server);
        file.print("user:");
        file.println(user);
        file.print("pass:");
        file.println(pass);
        file.print("devName:");
        file.println(devName);
        file.close();       
        
        serialLogln(filename);
        serialLog("server:");
        serialLogln(server);
        serialLog("user:");
        serialLogln(user);
        serialLog("pass:");
        serialLogln(pass);
        serialLog("devName:");
        serialLogln(devName);
        serialLogln("Datei gespeichert");
    }

    static bool fileExist(const char* filename){
        serialLog("looking for file...");

        return SPIFFS.exists(filename);
    }

    static char* fileParseString(File file, const char* key){
        
        char buffer[255];
        int len;
        char del = ':';
        char c;

        int i;
        char foundKey[10];
        char foundVal[255];

        while(file.available()){
            i = 0;
            while((c = file.read()) != '\n'){
                buffer[i] = c;
                i++;
            }   
            buffer[i] = '\0';
            i = 0;
            while(i < strlen(buffer)){
                if(buffer[i] == del){
                    memcpy(foundKey, buffer, i);
                    memcpy(foundVal, &buffer[i+1], strlen(buffer)-i);
                    foundKey[i] = '\0';
                    break;
                }
                i++;
            }
            if(strcmp(key,foundKey)==0){
                int len = strlen(foundVal);
                char *result = (char*)malloc(len);
                strncpy(result,foundVal,len-1); 
                result[len-1] = '\0'; 
                serialLogln(buffer);
                return result;
            }
        }
        return NULL;
    }

    static bool fileValid(const char* filename){
        serialLog("checking file...");
        File file = SPIFFS.open(filename, "r");
        if(strcmp(filename, WIFI_FILE)==0){
            const char * ssid = fileParseString(file,"ssid");
            const char * pass = fileParseString(file,"pass");
            return ((ssid != NULL)&&(pass != NULL));
        }
        if(strcmp(filename, MQTT_FILE)==0){
            const char * server = fileParseString(file,"server");
            const char * user = fileParseString(file,"user");
            const char * pass = fileParseString(file,"pass");
            const char * devName = fileParseString(file,"devName");
            return ((server != NULL)&&(user != NULL)&&(pass != NULL)&&(devName != NULL));
        }

    }

    static void askFromTelnet(const char* msg, char* ans){
        #if MODE == 1
            printf("%s", msg);
            scanf("%s", &ans);
            return;
        #endif
    }

    static void sendToTelnet(const char* msg){
        #if MODE == 1
            printf("%s", msg);
            return;
        #endif
    }


#endif
