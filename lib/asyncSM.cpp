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

void asyncSM::setServer(AsyncWebServer httpServer){
    _httpServer = &httpServer;
}

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
    debug::logln("entering: sm_wifi_look_for_data");
    if (tools::fileExist(WIFI_FILE)){
        debug::logln("WiFi-File found...");
        if (true){
            debug::logln("WiFi-File ok...");
            _state = WIFI_LOGIN;
            return;
        }else{
            debug::logln("WiFi-File corrupted...");
            _state = WIFI_OPEN_ACCESSPOINT;
            return;
        }
    }else{
        debug::logln("WiFi-File not found...");
        _state = WIFI_OPEN_ACCESSPOINT;
        return;
    }
    _state = UNRESOLVABLE_ERROR;
    return;
}

void asyncSM::_wifiLogin(){
    debug::logln("entering: sm_wifi_login");
    _loadWifiConfig();

    debug::logln("connectiong to:");
    debug::log("  SSID: ");
    debug::logln(_wifiConfig.ssid);

    WiFi.begin(_wifiConfig.ssid.c_str(), _wifiConfig.pass.c_str());
    long t0 = millis();

    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        debug::log(".");
        if((millis() - t0)/1000 > WIFI_TIMEOUT){
            _state = WIFI_OPEN_ACCESSPOINT;
            return;
        }
    }
    debug::logln("WiFi connected.");
    debug::log("  IP: ");
    debug::logln(WiFi.localIP().toString());
    _state = MQTT_LOOK_FOR_DATA;
    return;
}

void asyncSM::_wifiOpenAccessPoint(){
    debug::logln("entering: sm_wifi_open_accesspoint");
    const char *ssid = "WIFI_AP_SSID";
    const char *pass = "WIFI_AP_PASS";
    delay(1000);
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid);
    delay(1000);
    _state = WIFI_WAIT_FOR_CONFIG;
    Serial.println(WiFi.localIP());
    return;
}

void asyncSM::_wifiWaitForConfig(){
    debug::logln("entering: sm_wifi_wait_for_config");
    _httpServer->begin();
    _runningState = PAUSED;
}

void asyncSM::_mqttLookForData(){

}

void asyncSM::_mqttLogin(){

}

void asyncSM::_mainHandleMqtt(){

}

void asyncSM::_loadWifiConfig(){
    JsonObject& wifidata = tools::loadJsonFile(WIFI_FILE);
    _wifiConfig.ssid = wifidata["SSID"].as<String>();
    _wifiConfig.pass = wifidata["PASS"].as<String>();
}

void asyncSM::saveWifiConfig(){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["SSID"] = _wifiConfig.ssid;
    root["PASS"] = _wifiConfig.pass;
    tools::saveJsonFile(WIFI_FILE, &root);
}

void asyncSM::setWifiSSID(String ssid){
    _wifiConfig.ssid = ssid;
}

void asyncSM::setWifiPass(String pass){
    _wifiConfig.pass = pass;
}