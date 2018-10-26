#include "hardwareManager.h"

String HardwareIO::getName(){
    return _name;
}

eHardwareType HardwareIO::getType(){
    return _type;
}

String HardwareIO::getMqttFragment(){
    return _mqttFragment;
}

String HardwareIO::getMqttTopic(){
    String s;
    if(_type == RELAY)
        s = "cmnd/";
    else if(_type == INPUT230V)
        s = "stat/";
    else if(_type == TELEMETRY)
        s = "tele/";

    s += asyncSM::getInstance()->getMqttDevName() + "/" + _mqttFragment;
     
    return s;
}

String HardwareIO::getWebDesc(){
    return _webDesc;
}


HardwareRelay::HardwareRelay(int pin, String name, String mqttFragment){
    _pin = pin;
    _name = name;
    _mqttFragment = mqttFragment;
    _type = RELAY;
    _webDesc = String("Relais, erlaubte MQTT-Payloads: ON, OFF");
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void HardwareRelay::setState(eRelayState state){
    if(state == ON)
        digitalWrite(_pin, HIGH);
    else
        digitalWrite(_pin, LOW);
}

eRelayState HardwareRelay::getState(){
    if(digitalRead(_pin) == HIGH)
        return ON;
    else
        return OFF;
}

HardwareTele::HardwareTele(String name, String mqttFragment, String (*getVal)(), String webDesc){
    _name = name;
    _mqttFragment = mqttFragment;
    _type = TELEMETRY;
    _getVal = getVal;
    _webDesc = webDesc;
}

String HardwareTele::getValue(){
    return _getVal();
}


HardwareList::HardwareList(){
    _list[0] = new HardwareRelay(16, "Relais 1", "relay1");
    _list[1] = new HardwareRelay(17, "Relais 2", "relay2");
    _list[2] = new HardwareTele("RSSI", "rssi", [](){return String(WiFi.RSSI());}, "WiFi-Signalstaerke");
}

int HardwareList::getLen(){
    return _len;
}

HardwareIO* HardwareList::getElement(int i){
    return _list[i];
}