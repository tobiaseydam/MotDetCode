//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>

#include "asyncSM.h"
#include "debug.h"
#include "tools.h"

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
    debug::logln(_wifiConfig.ssid);

    WiFi.begin(_wifiConfig.ssid, _wifiConfig.pass);
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
    _mqttClient->setCredentials("pi", "raspberry");
    _mqttClient->connect();
    _mqttTimer->attach(getMqttTimerIntervallSeconds(), _handleTeleHardware); 
    _mqttTimer2->attach(1, _handleSensorHardware); 
    _runningState = PAUSED;
    return;
}

void asyncSM::_mainHandleMqtt(){

}

void asyncSM::_loadWifiConfig(){
    if(_wifiConfig.loaded) return;
    JsonObject& wifidata = tools::loadJsonFile(WIFI_FILE);
    strncpy(_wifiConfig.ssid, wifidata["SSID"], sizeof(_wifiConfig.ssid));
    strncpy(_wifiConfig.pass, wifidata["PASS"], sizeof(_wifiConfig.pass));
    _wifiConfig.loaded = true;
}

void asyncSM::_loadMqttConfig(){
    if(_mqttConfig.loaded) return;
    JsonObject& mqttdata = tools::loadJsonFile(MQTT_FILE);
    strncpy(_mqttConfig.server, mqttdata["SERVER"], sizeof(_mqttConfig.server));
    strncpy(_mqttConfig.user, mqttdata["USER"], sizeof(_mqttConfig.user));
    strncpy(_mqttConfig.pass, mqttdata["PASS"], sizeof(_mqttConfig.pass));
    strncpy(_mqttConfig.devname, mqttdata["DEVNAME"], sizeof(_mqttConfig.devname));
    _mqttConfig.loaded = true;
}

void asyncSM::_handleTeleHardware(){
    if(true){
        HardwareList* _hwl = asyncSM::getInstance()->_hardware;
        for(int i = 0; i < _hwl->getLen(); i++){
            HardwareIO* h = _hwl->getElement(i);
            if(h->getType() == TELEMETRY){
                HardwareTele* t = (HardwareTele*)h;
                asyncSM::getInstance()->_mqttClient->publish(t->getMqttTopic().c_str(), 1, true, t->getValue().c_str());
            }
            if(h->getType() == ONEWIRE){
                Hardware1WireSensor* t = (Hardware1WireSensor*)h;
                Serial.println(t->getMqttTopic().c_str());
                Serial.println(t->getValue().c_str());
                asyncSM::getInstance()->_mqttClient->publish(t->getMqttTopic().c_str(), 1, true, t->getValue().c_str());
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
                Hardware230VSensor* t = (Hardware230VSensor*)h;
                t->read();
                if(t->hasChanged()){
                    asyncSM::getInstance()->_mqttClient->publish(t->getMqttTopic().c_str(), 1, true, t->getValue().c_str());
                }
            }
        }
    }
}

void asyncSM::saveWifiConfig(){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["SSID"] = _wifiConfig.ssid;
    root["PASS"] = _wifiConfig.pass;
    String s = "/wifi.txt";
    tools::saveJsonFile(s, &root);
}

void asyncSM::saveMqttConfig(){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["SERVER"] = _mqttConfig.server;
    root["USER"] = _mqttConfig.user;
    root["PASS"] = _mqttConfig.pass;
    root["DEVNAME"] = _mqttConfig.devname;
    String s = "/mqtt.txt";
    tools::saveJsonFile(s, &root);
}

void asyncSM::setWifiSSID(String ssid){
    strncpy(_wifiConfig.ssid, ssid.c_str(), sizeof(_wifiConfig.ssid));
}

String asyncSM::getWifiSSID(){
    _loadWifiConfig();
    String res = _wifiConfig.ssid;
    return res;
}

void asyncSM::setWifiPass(String pass){
    strncpy(_wifiConfig.pass, pass.c_str(), sizeof(_wifiConfig.pass));
}

String asyncSM::getWifiPass(){
    _loadWifiConfig();
    String res = _wifiConfig.pass;
    return res;
}

void asyncSM::setMqttServer(String server){
    strncpy(_mqttConfig.server, server.c_str(), sizeof(_mqttConfig.server));
}

String asyncSM::getMqttServer(){
    _loadMqttConfig();
    String res = _mqttConfig.server;
    return res;
}

void asyncSM::setMqttUser(String user){
    strncpy(_mqttConfig.user, user.c_str(), sizeof(_mqttConfig.user));
}

String asyncSM::getMqttUser(){
    _loadMqttConfig();
    String res = _mqttConfig.user;
    return res;
}

void asyncSM::setMqttPass(String pass){
    strncpy(_mqttConfig.pass, pass.c_str(), sizeof(_mqttConfig.pass));
}

String asyncSM::getMqttPass(){
    _loadMqttConfig();
    String res = _mqttConfig.pass;
    return res;
}

void asyncSM::setMqttDevName(String devname){
    strncpy(_mqttConfig.devname, devname.c_str(), sizeof(_mqttConfig.devname));
}

String asyncSM::getMqttDevName(){
    _loadMqttConfig();
    String res = _mqttConfig.devname;
    return res;
}

String asyncSM::getMqttOneWireName(String addr){
    JsonObject& mqttdata = tools::loadJsonFile(MQTT_FILE);
    char name[32];
    strncpy(name, mqttdata[addr], sizeof(mqttdata[addr]));
    return String(name);
}

String asyncSM::getMqttTimerIntervall(){
    return String(getMqttTimerIntervallSeconds());
}

int asyncSM::getMqttTimerIntervallSeconds(){
    return 10;
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
        res += "  Type: ";
        if(h->getType() == RELAY)
            res += "RELAY\n";
        else if(h->getType() == INPUT230V)
            res += "INPUT230V\n";
        else if(h->getType() == TELEMETRY)
            res += "TELEMETRY\n";
        res += "  MQTT-Topic: " + h->getMqttTopic() + "\n";
        res += "  Desc: " + h->getWebDesc() + "\n";       
    }
    return res;
}

String asyncSM::getWebHardwareInfo(){
    String res = "<table><tr><td>Element</td><td>Type</td><td>MQTT-Topic</td><td>Beschreibung</td></tr>\n";
    for (int i = 0; i<_hardware->getLen(); i++){
        HardwareIO* h = _hardware->getElement(i);
        res += "<tr><td>" + h->getName() + "</td><td>";
        if(h->getType() == RELAY)
            res += "RELAY";
        else if(h->getType() == INPUT230V)
            res += "INPUT230V";
        else if(h->getType() == TELEMETRY)
            res += "TELEMETRY";
        else if(h->getType() == ONEWIRE)
            res += "ONEWIRE";
        res += "</td><td>" + h->getMqttTopic();
        res += "</td><td>" + h->getWebDesc() + "</td></tr>\n";       
    }
    res += "</table>";
    return res;
}

String asyncSM::getOneWireInfo(){
    String res = "<table><tr><td>Sensor</td><td>MQTT-Fragment</td></tr>\n";
    for (int i = 0; i<_hardware->getLen(); i++){
        HardwareIO* h = _hardware->getElement(i);
        if(h->getType() == ONEWIRE){
            Hardware1WireSensor* s = (Hardware1WireSensor*)h;
            for(int j = 0; j < s->getNumDevs(); j++){
                res = res + "<tr><td>" + s->_getAddr(j) + "</td>";
                res = res + "<td><input type=\"text\" name=\""+s->_getAddr(j)+"\" value=\"\"></td></tr>";
            }
        }
    }
    res += "</table>";
    return res;
}