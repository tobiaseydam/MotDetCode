#ifndef SM_WIFI_CPP
    #define SM_WIFI_CPP
        
    #include "SM_defines.h"
    #include "Utils.cpp"
    #include "WiFi.h"
    #include "time.h"

    static void wifi_ask(WiFiClient serverClient, char *msg);
    static void wifi_clear_input_buffer(WiFiClient serverClient);
    
    static void sm_wifi_look_for_data(void){
        serialLog("entering: sm_wifi_look_for_data");
        
        if(fileExist(WIFI_FILE)){
            serialLog("WiFi-File found...");
            
            if(fileValid(WIFI_FILE)){
                serialLog("WiFi-File ok...");
                sm_state = SM_WIFI_LOGIN;
                return;
            }else{
                serialLog("WiFi-File corrupted...");

                sm_state = SM_WIFI_OPEN_ACCESSPOINT;
                return;
            }

        }else{
            serialLog("WiFi-File not found...");
                
            sm_state = SM_WIFI_OPEN_ACCESSPOINT;
            return;
        }

        sm_state = SM_UNRESOLVABLE_ERROR;
        return;
    }


    static void sm_wifi_login(void){
        serialLog("entering: sm_wifi_login");
        File file = SPIFFS.open(WIFI_FILE, "r");
        const char * ssid = fileParseString(file,"ssid");
        const char * pass = fileParseString(file,"pass");

        serialLogln("connectiong to:");
        serialLog("SSID: ");
        serialLogln(ssid);

        WiFi.begin(ssid, pass);
        long t0 = millis();

        while(WiFi.status() != WL_CONNECTED){
            delay(500);
            serialLog(".");
            if((millis() - t0)/1000 > WIFI_TIMEOUT){
                sm_state = SM_WIFI_OPEN_ACCESSPOINT;
                return;
            }
        }
        //serialLogln();
        serialLogln("WiFi connected.");
        serialLog("IP: ");
        //serialLogln(WiFi.localIP());

        sm_state = SM_MQTT_LOOK_FOR_DATA;
        
        return;
    }


    static void sm_wifi_check(void){
        serialLog("entering: sm_wifi_check");

        sm_state = SM_MQTT_LOOK_FOR_DATA;
        return;
    }


    static void sm_wifi_open_accesspoint(void){
        serialLog("entering: sm_wifi_open_accesspoint");
        const char *ssid = "WIFI_AP_SSID";
        const char *pass = "WIFI_AP_PASS";
        WiFi.softAP(ssid, pass);
        sm_state = SM_WIFI_WAIT_FOR_CLIENT;
        return;
    }


    static void sm_wifi_wait_for_client(void){
        serialLog("entering: sm_wifi_wait_for_client");
        telnetServer.begin();
        WiFiClient serverClient;
        char ssid[32];
        char pass[32];

        while(true){
            if(telnetServer.hasClient()){
                if (!serverClient || !serverClient.connected()){    //  Wenn der Client nicht mehr existiert oder nicht mehr verbunden ist
                    if(serverClient) serverClient.stop();           //  Wenn es ihn noch gibt, dann stoppen
                    serverClient = telnetServer.available();        //  Slot neu befüllen
                    serialLog("Neuer Client");
                }
            }

            if (serverClient && serverClient.connected()){          //  Wenn Client connected
                serverClient.println("speak friend and enter...");
                serverClient.println("WiFi-Einstellungen:");  
                serverClient.print("  SSID: "); 
                delay(10);
                wifi_clear_input_buffer(serverClient);
                serverClient.flush();
                wifi_ask(serverClient, ssid);

                serverClient.print("  Pass: "); 
                wifi_ask(serverClient, pass);
            }

            if((ssid[0]!='\0')&&(pass[0]!='\0')){
                fileSave_WiFi(WIFI_FILE, ssid, pass);
                ESP.restart();
                break;
            }
        }

        telnetServer.stop();

        sm_state = SM_UNRESOLVABLE_ERROR;
        return;
    }

    static void wifi_ask(WiFiClient serverClient, char *msg){
        msg[0] = '\0';
        while(msg[0] == '\0'){
            uint8_t i = 0;
            while(serverClient.available()>0){                    //  Gucken, ob Zeichen verfügbar
                char c = serverClient.read();
                if((c != 10)&&(c != 13)){
                    msg[i] = c;
                    i++;
                }
            }
            msg[i] = '\0';
        }
    }

    static void wifi_clear_input_buffer(WiFiClient serverClient){
        while(serverClient.available()>0){
            serverClient.read();
        }
    }

    static void sm_wifi_ask_for_ssid(void){
        serialLog("entering: sm_wifi_ask_for_ssid");
        char* ssid;
        
            
        askFromTelnet("SSID: ", ssid);
            
        

        sm_state = SM_UNRESOLVABLE_ERROR;
        return;
    }


    static void sm_wifi_ask_for_pass(void){
        serialLog("entering: sm_wifi_ask_for_pass");

        
        return;
    }


    static void sm_wifi_save_data(void){
        serialLog("entering: sm_wifi_save_data");

        
        sm_state = SM_UNRESOLVABLE_ERROR;
        return;
    }

#endif