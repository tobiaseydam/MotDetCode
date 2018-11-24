#include "hardwareManager.h"

void HardwareRelay::setState(eState state){
    if(state == ON)
        digitalWrite(_pin, HIGH);
    else
        digitalWrite(_pin, LOW);
}

void HardwareRelay::toggle(){
    if(getState() == ON)
        setState(OFF);
    else
        setState(ON);
}

void HardwareRelay::handleMQTT(String payload){
    if(payload.equalsIgnoreCase("ON"))
        setState(ON);
    if(payload.equalsIgnoreCase("OFF"))
        setState(OFF);
    if(payload.equalsIgnoreCase("TOGGLE"))
        toggle();
}

eState HardwareRelay::getState(){
    if(digitalRead(_pin) == HIGH)
        return ON;
    else
        return OFF;
}

String HardwareRelay::getTxtState(){
    if(getState() == ON)
        return String("ON");
    else
        return String("OFF");
}

HardwareRelayOmronG5LE14::HardwareRelayOmronG5LE14(int pin, String name, String mqttFragment2){
    _pin = pin;
    _name = name;
    _mqttFragment0 = String("cmnd");
    _mqttFragment2 = mqttFragment2;
    _type = RELAY;
    _txtDesc = String("Relais, 230V, 10A, MQTT-Payloads: ON, OFF, TOGGLE");
    _constUpdate = false;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}