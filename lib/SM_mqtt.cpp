#ifndef SM_MQTT_CPP
    #define SM_MQTT_CPP

    #include "SM_defines.h"
    #include "Utils.cpp"


    static void sm_mqtt_look_for_data(void){
        serialLog("entering: sm_mqtt_look_for_data");
        
        if(fileExist(MQTT_FILE)){
            serialLog("MQTT-File found...");

            if(fileValid(MQTT_FILE)){
                serialLog("MQTT-File ok...");
                sm_state = SM_MQTT_LOGIN;
                return;
            }else{
                serialLog("MQTT-File corrupted...");
                sm_state = SM_MQTT_WAIT_FOR_CLIENT;
                return;
            }
        }else{
            serialLog("MQTT-File not found...");
            sm_state = SM_MQTT_WAIT_FOR_CLIENT;
            return;
        }
        
        sm_state = SM_UNRESOLVABLE_ERROR;
        return;
    }


    static void sm_mqtt_login(void){
        serialLog("entering: sm_mqtt_login");

        File file = SPIFFS.open(MQTT_FILE, "r");
        const char *server = fileParseString(file,"server");
        const char *user = fileParseString(file,"user");
        const char *pass = fileParseString(file,"pass");
        const char *devName = fileParseString(file,"devName");

        serialLogln("connectiong to:");
        serialLog("MQTT-Server: ");
        serialLogln(server);

        mqttClient.setServer(server, 1883);
        mqttClient.connect(devName, user, pass);

        if(mqttClient.connected()){
            serialLogln("connected...");
            sm_state = SM_MAIN_HANDLE_MQTT;
        }else{
            serialLogln("failed...");
            //serialLogln(mqttClient.state());
            sm_state = SM_MQTT_WAIT_FOR_CLIENT;
        }
        delay(1000);
        return;
    }


    static void sm_mqtt_check(void){
        serialLog("entering: sm_mqtt_check");

        
        return;
    }


    static void sm_mqtt_wait_for_client(void){
        serialLog("entering: sm_mqtt_wait_for_client");
        telnetServer.begin();
        WiFiClient serverClient;
        char server[32];
        char user[32];
        char pass[32];
        char devName[32];
        server[0] = '\0';
        user[0] = '\0';
        pass[0] = '\0';
        devName[0] = '\0';

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
                serverClient.println("MQTT-Einstellungen:");  
                serverClient.print("  Server: "); 
                delay(10);
                wifi_clear_input_buffer(serverClient);
                serverClient.flush();
                wifi_ask(serverClient, server);

                serverClient.print("  User: "); 
                wifi_ask(serverClient, user);

                serverClient.print("  Pass: "); 
                wifi_ask(serverClient, pass);

                serverClient.print("  MQTT-Device-name: "); 
                wifi_ask(serverClient, devName);
            }

            if((server[0]!='\0')&&(user[0]!='\0')&&(pass[0]!='\0')&&(devName[0]!='\0')){
                fileSave_MQTT(MQTT_FILE, server, user, pass, devName);
                ESP.restart();
                break;
            }
        }

        telnetServer.stop();

        sm_state = SM_UNRESOLVABLE_ERROR;
        return;
    }


    static void sm_mqtt_ask_for_server(void){
        serialLog("entering: sm_mqtt_ask_for_server");

        
        return;
    }


    static void sm_mqtt_ask_for_name(void){
        serialLog("entering: sm_mqtt_ask_for_name");

        
        return;
    }


    static void sm_mqtt_ask_for_pass(void){
        serialLog("entering: sm_mqtt_ask_for_pass");

        
        return;
    }


    static void sm_mqtt_ask_for_device_name(void){
        serialLog("entering: sm_mqtt_ask_for_device_name");

        
        return;
    }


    static void sm_mqtt_ask_for_topics(void){
        serialLog("entering: sm_mqtt_ask_for_topics");

        
        return;
    }


    static void sm_mqtt_save_data(void){
        serialLog("entering: sm_mqtt_save_data");

        
        return;
    }

#endif