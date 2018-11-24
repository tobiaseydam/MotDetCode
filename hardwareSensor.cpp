#include "hardwareManager.h"

void HardwareDigitalSensorPin::_read(){
    eState currentState;
    if(digitalRead(_pin) == HIGH){
        currentState = ON;
    }else{
        currentState = OFF;
    }
    if(currentState != _lastState){
        _changed = true;
        _lastState = currentState;
    }
}

eState HardwareDigitalSensorPin::getState(){
    return _lastState;
}

String HardwareDigitalSensorPin::getTxtState(){
    switch (_lastState){
        case ON:  return String("ON");
        case OFF: return String("OFF");
    }
    return String("NOT DEFINED");
}

boolean HardwareDigitalSensorPin::hasChanged(){
    return _changed;
}

void HardwareDigitalSensorPin::handleMQTT(String payload){
    return;
}

Hardware230VSensor::Hardware230VSensor(int pin, String name, String mqttFragment2){
    _pin = pin;
    _name = name;
    _mqttFragment0 = String("stat");
    _mqttFragment2 = mqttFragment2;
    _type = INPUT230V;
    _txtDesc = String("230V In, nur lesen");
    _constUpdate = false;
    pinMode(pin, INPUT_PULLUP);
}