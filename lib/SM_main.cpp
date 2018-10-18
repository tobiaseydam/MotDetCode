#ifndef SM_MAIN_CPP
    #define SM_MAIN_CPP
        
    #include "SM_defines.h"
    #include "Utils.cpp"
    #include "mainproc.cpp"

    #include "FS.h"
    #include <SPIFFS.h>

    #include "httpSPIFFS.cpp"

    void callback(char* topic, byte* payload, unsigned int length);

    static void sm_start(void){
        #ifdef DEBUG
            Serial.begin(115200);
        #endif
        serialLog("entering: sm_start");
        if(!SPIFFS.begin(true)){
            serialLogln("SPIFFS Mount failed...");
            sm_state = SM_UNRESOLVABLE_ERROR;
        }
        httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            serialLogln("neuer Client auf :80");
            request->send(200, "text/plain", "Hello World");
        });

        httpServer.on("/SPIFFS", HTTP_GET, handleSPIFFS);
        httpServer.on("/UPDATE", HTTP_GET, handleUpdate);

        httpServer.on("/UPLOAD", HTTP_POST, [](AsyncWebServerRequest * request) {
            request->redirect("/SPIFFS");
        }, handleUpload);

        sm_state = SM_WIFI_LOOK_FOR_DATA;
            return;
        }


    static void sm_main_handle_mqtt(void){
        serialLog("entering: sm_main_handle_mqtt");
        
        httpServer.begin();

        mqttClient.setCallback(callback);

        File file = SPIFFS.open(MQTT_FILE, "r");
        const char *devName = fileParseString(file,"devName");

        char top1[128] = "";
        char top2[128] = "";
        char top3[128] = "";
        sprintf(top1, "cmnd/%s/relay1", devName);
        sprintf(top2, "cmnd/%s/relay2", devName);
        sprintf(top3, "cmnd/%s/cmnd", devName);

        mqttClient.subscribe(top1);
        mqttClient.subscribe(top2);
        mqttClient.subscribe(top3);

        initSensors();
        int i = 0;
        while(true){
            checkSensors(devName);
            if(i>=60){
                checkState(devName);
                i = 0;
            }
            mqttClient.loop();
            delay(1000);
            i++;
            if(sm_state != SM_MAIN_HANDLE_MQTT){
                break;
            }
        }
        
        return;
    }

    static void sm_main_switch_relays(void){
        serialLog("entering: sm_main_switch_relays");

        
        return;
    }


    static void sm_main_check_motions(void){
        serialLog("entering: sm_main_check_motions");

        
        return;
    }


    static void sm_main_check_state(void){
        serialLog("entering: sm_main_check_state");

        
        return;
    }


    static void sm_main_send_mqtt(void){
        serialLog("entering: sm_main_send_mqtt");

        
        return;
    }


    static void sm_main_config_enter(void){
        serialLog("entering: sm_main_config_enter");

        
        return;
    }


    static void sm_main_config_edit_wifi(void){
        serialLog("entering: sm_main_config_edit_wifi");

        
        return;
    }


    static void sm_main_config_edit_mqtt(void){
        serialLog("entering: sm_main_config_edit_mqtt");

        
        return;
    }


    static void sm_main_config_reboot(void){
        serialLog("entering: sm_main_config_reboot");

        
        return;
    }

#endif
