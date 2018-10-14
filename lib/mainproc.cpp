#ifndef MAINPROC_CPP
    #define MAINPROC_CPP

    #include "SM_defines.h"
    #include "Utils.cpp"
    #ifdef __cplusplus
        extern "C" {
    #endif
    uint8_t temprature_sens_read();
    #ifdef __cplusplus
        }
    #endif

    void initSensors(){
        pinMode(IO_MODET_0, INPUT); 
        pinMode(IO_MODET_1, INPUT); 
        pinMode(IO_MODET_2, INPUT); 
        pinMode(IO_MODET_3, INPUT); 
        pinMode(IO_REL_0, OUTPUT);
        pinMode(IO_REL_1, OUTPUT);
    }

    boolean readSensorState(int channel){
        int channels[] = {IO_MODET_0, IO_MODET_1, IO_MODET_2, IO_MODET_3};
        boolean result = digitalRead(channels[channel])==0;
        return result;
    }

    void checkSensors(const char *devName){
        
        char str[2];
        for(int i = 0; i<4; i++){
            if((sensor_states[i] != readSensorState(i))){
                char topic[128] = "";
                sensor_states[i] = readSensorState(i);
                strcat(topic, "tele/");
                strcat(topic, devName);
                strcat(topic, "/sensor");
                sprintf(str, "%d", (i+1));
                strcat(topic, str);
                sprintf(str, "%d", sensor_states[i]);
                if(!mqttClient.connected()){
                    mqttClient.connect(devName);
                }
                mqttClient.publish(topic, str);
                serialLog(topic);
                serialLog(" ");
                serialLogln(str);
            }
        }
    }

    void checkState(const char *devName){
        char topic[128] = "";
        char str[10] = "";
        strcat(topic, "tele/");
        strcat(topic, devName);
        strcat(topic, "/rssi");
        sprintf(str, "%d", WiFi.RSSI());
        mqttClient.publish(topic, str);
        serialLog(topic);
        serialLog(" ");
        serialLogln(str);

        char topic2[128] = "";
        char str2[10] = "";
        strcat(topic2, "tele/");
        strcat(topic2, devName);
        strcat(topic2, "/temperature");
        sprintf(str2, "%f", (temprature_sens_read()-32)/1.8);
        mqttClient.publish(topic2, str2);
        serialLog(topic2);
        serialLog(" ");
        serialLogln(str2);
    }

    void setRelay(int i, boolean val){
        int channels[] = {IO_REL_0, IO_REL_1};
        if(val){
            digitalWrite(channels[i], HIGH);
        }else{
            digitalWrite(channels[i], LOW);
        }
    }

    void callback(char* topic, byte* payload, unsigned int length) {
 
        serialLog("Message arrived in topic: ");
        serialLogln(topic);
        
        serialLog("Message:");
        char pl[32] = "";
        for (int i = 0; i < length; i++) {
            //serialLog((char)payload[i]);
            pl[i] = payload[i];
        }
        pl[length] = '\0';
        
        int i = 0;
        int i1 = -1;
        int i2 = -1;
        char type[32] = "";
        char devName[32] = "";
        char cmd[32] = "";

        while(i < strlen(topic)){
            if(topic[i] == '/'){
                if(i1 == -1){
                    i1 = i;
                }else{
                    i2 = i;
                    break;
                }
            }
            i++;
        }
        
        memcpy(type, topic, i1);
        memcpy(devName, &topic[i1+1], i2-i1-1);
        memcpy(cmd, &topic[i2+1], strlen(topic)-i2);
        type[i1] = '\0';
        devName[i2-i1-1] = '\0';
        cmd[strlen(topic)-i2] =  '\0';

        serialLogln(type);
        serialLogln(devName);
        serialLogln(cmd);
        serialLogln(pl);

        if(strcmp(type,"cmnd")==0){
            if(strcmp(cmd,"relay1")==0){
                if(strcmp(pl, "ON")==0){
                    setRelay(0,true);
                }else{
                    setRelay(0,false);
                }
            }else if(strcmp(cmd,"relay2")==0){
                if(strcmp(pl, "ON")==0){
                    setRelay(1,true);
                }else{
                    setRelay(1,false);
                }
            }else if(strcmp(cmd,"cmnd")==0){
                if(strcmp(pl, "reboot")==0){
                    ESP.restart();
                }else if(strcmp(pl, "resetMQTT")==0){
                    sm_state = SM_MQTT_WAIT_FOR_CLIENT;
                }else if(strcmp(pl, "resetWIFI")==0){
                    ESP.restart();
                }
            }
        }

        //serialLogln();
        serialLogln("-----------------------");
        
    }

#endif