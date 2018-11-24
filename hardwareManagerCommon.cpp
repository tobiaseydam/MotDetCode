#include "hardwareManager.h"

String HardwareIO::getTxtName(){
    return _name;
}

String HardwareIO::getTxtType(){
    switch(_type){
        case RELAY:     return String("RELAY");
        case INPUT230V: return String("INPUT230V");
        case TELEMETRY: return String("TELEMETRY");
        case ONEWIRE:   return String("ONEWIRE");
    }
    return String("NOT DEFINED");
}

String HardwareIO::getTxtDesc(){
    return _txtDesc;
}

String HardwareIO::getMqttFragment(byte i){
    switch(i){
        case 0: return String(_mqttFragment0);
        case 1: return String(asyncSM::getInstance()->getMqttDevName());
        case 2: return String(_mqttFragment2);
    }
    return String("NOT_DEFINED");
}

String HardwareIO::getMqttTopic(){
    String topic = "";
    topic += getMqttFragment(0) + "/";
    topic += getMqttFragment(1) + "/";
    topic += getMqttFragment(2);
    return topic; 
}

String HardwareIO::getName(){
    return _name;
}

eHardwareType HardwareIO::getType(){
    return _type;
}

boolean HardwareIO::isConstUpdate(){
    return _constUpdate;
}

String HardwareIO::getTxtState(){
    return String("");
}

void HardwareIO::handleMQTT(String payload){
    return;
}

HardwareList::HardwareList(){
    _list[0] = new HardwareRelayOmronG5LE14(16, "Relais 1", "relay1");
    _list[1] = new HardwareRelayOmronG5LE14(17, "Relais 2", "relay2");
    _list[2] = new HardwareTeleRSSI("RSSI", "rssi");
    _list[3] = new HardwareTeleTemperature("Temperatur", "temperature");
    _list[4] = new Hardware230VSensor(15, "Sensor 230V 1", "sensor1");
    _list[5] = new Hardware230VSensor( 2, "Sensor 230V 2", "sensor2");
    _list[6] = new Hardware230VSensor( 0, "Sensor 230V 3", "sensor3");
    _list[7] = new Hardware230VSensor( 4, "Sensor 230V 4", "sensor4");
    _list[8] = new HardwareOneWireSensor( 5, "OneWire", "onewire");
}

int HardwareList::getLen(){
    return _len;
}

HardwareIO* HardwareList::getElement(int i){
    return _list[i];
}