//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>

#include "asyncSM.h"
#include "debug.h"
#include "tools.h"

void asyncSM::begin(void *pvParameter){
    _nextStep();
}

//void asyncSM::setServer(AsyncWebServer httpServer){
    //_httpServer = httpServer;
//}

asyncSM::asyncSM(void){
    debug::init();
    tools::init();
}

void asyncSM::_nextStep(){
    while(_runningState!=PAUSED){
        switch(_state){
            case START:  
                _start();
                break;
            case WIFI_LOOK_FOR_DATA:
                _wifiLookForData();
                break;
            case WIFI_LOGIN:
                _wifiLogin();
                break;
            case WIFI_OPEN_ACCESSPOINT:
                _wifiOpenAccessPoint();
                break;
            case WIFI_WAIT_FOR_CONFIG:
                _wifiWaitForConfig();
                break;
            case MQTT_LOOK_FOR_DATA:
                _mqttLookForData();
                break;
            case MQTT_LOGIN:
                _mqttLogin();
                break;
            case MAIN_HANDLE_MQTT:
                _mainHandleMqtt();
                break;
        }
    }
}

void asyncSM::_start(){
        debug::logln("entering: sm_start");
        _runningState = RUNNING;
        _state = WIFI_LOOK_FOR_DATA;
}

void asyncSM::_wifiLookForData(){
    debug::log("entering: sm_wifi_look_for_data");
    if (tools::fileExist(WIFI_FILE)){
        debug::log("WiFi-File found...");
        if (true){
            debug::log("WiFi-File ok...");
            _state = WIFI_LOGIN;
            return;
        }else{
            debug::log("WiFi-File corrupted...");
            _state = WIFI_OPEN_ACCESSPOINT;
            return;
        }
    }else{
        debug::log("WiFi-File not found...");
        _state = WIFI_OPEN_ACCESSPOINT;
        return;
    }
    _state = UNRESOLVABLE_ERROR;
    return;
}

void asyncSM::_wifiLogin(){
    debug::log("entering: sm_wifi_login");
    JsonObject& foo = tools::loadJsonFile("/wifi.txt");
    String ssid = foo["SSID"];
    String pass = foo["PASS"];

    debug::log("connectiong to:");
    debug::log("SSID: ");
    debug::log(ssid);

    WiFi.begin(ssid.c_str(), pass.c_str());
    long t0 = millis();

    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        debug::log(".");
        if((millis() - t0)/1000 > WIFI_TIMEOUT){
            _state = WIFI_OPEN_ACCESSPOINT;
            return;
        }
    }
    debug::log("WiFi connected.");
    debug::log("IP: ");
    //serialLogln(WiFi.localIP());
    _state = MQTT_LOOK_FOR_DATA;
    return;
}

void asyncSM::_wifiOpenAccessPoint(){
    debug::log("entering: sm_wifi_open_accesspoint");
    String ssid = "WIFI_AP_SSID";
    String pass = "WIFI_AP_PASS";
    WiFi.softAP(ssid.c_str(), pass.c_str());
    _state = WIFI_WAIT_FOR_CONFIG;
    return;
}

void asyncSM::_wifiWaitForConfig(){
    //_httpServer.begin();
    _runningState = PAUSED;
}

void asyncSM::_mqttLookForData(){

}

void asyncSM::_mqttLogin(){

}

void asyncSM::_mainHandleMqtt(){

}
