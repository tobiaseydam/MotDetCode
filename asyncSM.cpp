//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>

#include "asyncSM.h"
#include "debug.h"
#include "tools.h"
#include "mqttServer.h"
asyncSM* asyncSM::asyncSMInstance;

asyncSM* asyncSM::getInstance(){
    if(!asyncSMInstance){
        asyncSMInstance = new asyncSM();
    }
    return asyncSMInstance;
}

void asyncSM::begin(void *pvParameter){
    _nextStep();
}

void asyncSM::setHttpServer(AsyncWebServer *httpServer){
    _httpServer = httpServer;
}

AsyncWebServer *asyncSM::getHttpServer(){
    return _httpServer;
}

void asyncSM::setMqttClient(AsyncMqttClient *mqttClient){
    _mqttClient = mqttClient;
}

AsyncMqttClient *asyncSM::getMqttClient(){
    return _mqttClient;
}

asyncSM::asyncSM(void){
    debug::init();
    tools::init();
    _httpServer = new AsyncWebServer(80);
    _mqttClient = new AsyncMqttClient();
    _hardware = new HardwareList();
    Serial.println("AsyncSM created");
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
        //Serial.println(getHardwareInfo());
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
    debug::logln(getWifiConfigKey("SSID"));

    WiFi.begin(getWifiConfigKey("SSID").c_str(), getWifiConfigKey("PASS").c_str());
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
    _httpServer->begin();
    _state = MQTT_LOOK_FOR_DATA;
    return;
}

void asyncSM::_wifiOpenAccessPoint(){
    debug::logln("entering: sm_wifi_open_accesspoint");
    
    const char *ssid = "WIFI_AP_SSID";
    const char *pass = "WIFI_AP_PASS";
    WiFi.softAP(ssid);
 
    Serial.println(WiFi.softAPIP());
    _state = WIFI_WAIT_FOR_CONFIG;
    return;
}

void asyncSM::_wifiWaitForConfig(){
    debug::logln("entering: sm_wifi_wait_for_config");
    _httpServer->begin();
    _runningState = PAUSED;
}

void asyncSM::_mqttLookForData(){
    debug::logln("entering: sm_mqtt_look_for_data");
    if (tools::fileExist(MQTT_FILE)){
        debug::logln("MQTT-File found...");
        if (true){
            debug::logln("MQTT-File ok...");
            _state = MQTT_LOGIN;
            return;
        }else{
            debug::logln("WiFi-File corrupted...");
            _runningState = PAUSED;
            return;
        }
    }else{
        debug::logln("MQTT-File not found...");
        _runningState = PAUSED;
        return;
    }
    _state = UNRESOLVABLE_ERROR;
    return;
}

void asyncSM::_mqttLogin(){
    debug::logln("entering: sm_mqtt_login");
    _loadMqttConfig();
    mqttServerBuilder::init();
    _mqttClient->setCredentials("pi", "raspberry");
    _mqttClient->connect();
    _mqttTimer->attach(10, _handleTeleHardware); 
    _mqttTimer2->attach(1, _handleSensorHardware); 
    _runningState = PAUSED;
    return;
}

void asyncSM::_mainHandleMqtt(){

}

void asyncSM::_loadWifiConfig(){
    JsonObject& data = tools::loadJsonFile(WIFI_FILE);
    for (auto kv : data) {
        Serial.println(kv.key + String(" - ") + kv.value.as<char*>());
        _wifiConfigMap[kv.key] = kv.value.as<char*>();
    }
}

void asyncSM::_loadMqttConfig(){
    JsonObject& data = tools::loadJsonFile(MQTT_FILE);
    for (auto kv : data) {
        Serial.println(kv.key + String(" - ") + kv.value.as<char*>());
        _mqttConfigMap[kv.key] = kv.value.as<char*>();
    }
}

void asyncSM::_handleTeleHardware(){
    if(true){
        HardwareList* _hwl = asyncSM::getInstance()->_hardware;
        for(int i = 0; i < _hwl->getLen(); i++){
            HardwareIO* h = _hwl->getElement(i);
            if(h->isConstUpdate()){
                Serial.println(h->getMqttTopic().c_str());
                Serial.println(h->getTxtState().c_str());
                asyncSM::getInstance()->_mqttClient->publish(h->getMqttTopic().c_str(), 1, true, h->getTxtState().c_str());
                if(h->getType() == ONEWIRE){
                    ((HardwareOneWireSensor*) h)->publishSensors();
                }
            }
        }
    }
}

void asyncSM::_handleSensorHardware(){
    if(true){
        HardwareList* _hwl = asyncSM::getInstance()->_hardware;
        for(int i = 0; i < _hwl->getLen(); i++){
            HardwareIO* h = _hwl->getElement(i);
            if(h->getType() == INPUT230V){
                HardwareDigitalSensorPin* t = (HardwareDigitalSensorPin*)h;
                t->read();
                if(t->hasChanged()){
                    asyncSM::getInstance()->_mqttClient->publish(t->getMqttTopic().c_str(), 1, true, t->getTxtState().c_str());
                }
            }
        }
    }
}

void asyncSM::saveWifiConfig(){
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    for(auto kv : _wifiConfigMap){
        root[kv.first] = kv.second;
    }
    String s = "/wifi.txt";
    tools::saveJsonFile(s, &root);
}

void asyncSM::saveMqttConfig(){
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    for(auto kv : (_mqttConfigMap)){
        root[kv.first] = kv.second;
    }
    String s = "/mqtt2.txt";
    tools::saveJsonFile(s, &root);
}


String asyncSM::getMqttOneWireName(String addr){
    JsonObject& mqttdata = tools::loadJsonFile(MQTT_FILE);
    Serial.println(addr);
    String name = getMqttConfigKey("ONEWIRE " + addr);
    Serial.println(name);
    return String(name);
}

Ticker* asyncSM::getMqttTimer(){
    return _mqttTimer;
}

String asyncSM::getHardwareInfo(){
    String res = "";
    res += "Hardware info: \n";
    for (int i = 0; i<_hardware->getLen(); i++){
        HardwareIO* h = _hardware->getElement(i);
        res += "Element: " + h->getName() + "\n";
        res += "  Type: " + h->getTxtType() + "\n";
        res += "  MQTT-Topic: " + h->getMqttTopic() + "\n";
        res += "  Desc: " + h->getTxtDesc() + "\n";       
    }
    return res;
}

String asyncSM::getWebHardwareInfo(){
    String res = "<table><tr><td>Element</td><td>Type</td><td>MQTT-Topic</td><td>Beschreibung</td></tr>\n";
    for (int i = 0; i<_hardware->getLen(); i++){
        HardwareIO* h = _hardware->getElement(i);
        res += "<tr>";
        res += "<td>" + h->getName() + "</td>";
        res += "<td>" + h->getTxtType() + "</td>";
        res += "<td>" + h->getMqttTopic() + "</td>";
        res += "<td>" + h->getTxtDesc() + "</td>";
        res += "</tr>\n";  
    }
    res += "</table>";
    return res;
}

String asyncSM::getOneWireInfo(){
    String res = "";
    for (int i = 0; i<_hardware->getLen(); i++){
        HardwareIO* h = _hardware->getElement(i);
        if(h->getType() == ONEWIRE){
            HardwareOneWireSensor* s = (HardwareOneWireSensor*)h;
            for(int j = 0; j < s->getNumDevs(); j++){
                res = res + "<tr><td>" + s->getAddr(j) + "</td>";
                res = res + "<td><input type=\"text\" name=\"ONEWIRE "+s->getAddr(j)+"\" value=\""+getMqttOneWireName(s->getAddr(j))+"\"></td></tr>";
            }
        }
    }
    res += "";
    return res;
}

String asyncSM::getWifiConfigKey(String key){
    return _wifiConfigMap[key];
}

void asyncSM::setWifiConfigKey(String key, String val){
    _wifiConfigMap[key] = val;
}

String asyncSM::getMqttConfigKey(String key){
    return (_mqttConfigMap)[key];
}

void asyncSM::setMqttConfigKey(String key, String val){
    (_mqttConfigMap)[key] = val;
}