#include "hardwareManager.h"


#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();
//uint8_t g_phyFuns;

#ifdef __cplusplus
}
#endif

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
    else if(_type == ONEWIRE)
        s = "onew/";

    s += asyncSM::getInstance()->getMqttDevName() + "/" + _mqttFragment;
     
    return s;
}

String HardwareIO::getWebDesc(){
    return _webDesc;
}

String HardwareIO::getStringState(){
    return String();
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

void HardwareRelay::setState(eState state){
    if(state == ON)
        digitalWrite(_pin, HIGH);
    else
        digitalWrite(_pin, LOW);
}

void HardwareRelay::setStringState(char* state){
    if(strcmp(state, "ON")==0){
        setState(ON);
    }else if(strcmp(state, "OFF")==0){
        setState(OFF);
    }
}

eState HardwareRelay::getState(){
    if(digitalRead(_pin) == HIGH)
        return ON;
    else
        return OFF;
}

String HardwareRelay::getStringState(){
    if(getState() == ON){
        return String("ON");
    }else if(getState() == OFF){
        return String("OFF");
    }
}

Hardware230VSensor::Hardware230VSensor(int pin, String name, String mqttFragment){
    _pin = pin;
    _name = name;
    _mqttFragment = mqttFragment;
    _type = INPUT230V;
    _webDesc = String("230V Input, nur lesen");
    pinMode(pin, INPUT_PULLUP);
}

eState Hardware230VSensor::getState(){
    if(digitalRead(_pin) == HIGH)
        return ON;
    else
        return OFF;
}

String Hardware230VSensor::getStringState(){
    if(getState() == ON){
        return String("ON");
    }else if(getState() == OFF){
        return String("OFF");
    }
}

boolean Hardware230VSensor::hasChanged(){
    boolean c = changed;
    changed = false;
    return c;
}

void Hardware230VSensor::read(){
    Serial.println(getState());
    if(lastState != getState()){
        changed = true;
        lastState = getState();
    }
}

String Hardware230VSensor::getValue(){
    return getStringState();
}

Hardware1WireSensor::Hardware1WireSensor(int pin, String name, String mqttFragment){
    _pin = pin;
    _name = name;
    _mqttFragment = mqttFragment;
    _type = ONEWIRE;
    _webDesc = String("1Wire Sensor, nur lesen");
    //pinMode(pin, INPUT_PULLUP);
    ds = new OneWire(_pin);
    for(byte j = 0; j<16; j++){
        if (!ds->search(addr[j])) {
            numDev = j;
            Serial.print("Keine weiteren Addressen.\n");
            Serial.println(j);
            ds->reset_search();
            return;
        }
        Serial.print("R=");
        for(byte i = 0; i < 8; i++) {
            Serial.print(addr[j][i], HEX);
            Serial.print(" ");
        }
        if ( OneWire::crc8( addr[j], 7) != addr[j][7]) {
            Serial.print("CRC nicht gültig!\n");
            return;
        }
        if ( addr[j][0] == 0x10) {
            Serial.print("Gerät ist aus der DS18S20 Familie.\n");
        }
        else if ( addr[j][0] == 0x28) {
            Serial.print("Gerät ist aus der DS18B20 Familie.\n");
        }
        else {
            Serial.print("Gerätefamilie nicht erkannt : 0x");
            Serial.println(addr[j][0],HEX);
            return;
        }
    }
}

float Hardware1WireSensor::_getSensorValue(byte k){
    byte data[12];
    byte present = 0;
    ds->reset();
    ds->select(addr[k]);
    ds->write(0x44,1);
    delay(1000); 
    present = ds->reset();
    ds->select(addr[k]);    
    ds->write(0xBE);
    for ( int i = 0; i < 9; i++) {           // 9 bytes
        data[i] = ds->read();
    }
    int HighByte, LowByte, TReading, SignBit, Tc_100;

    LowByte = data[0];
    HighByte = data[1];
    TReading = (HighByte << 8) + LowByte;
    SignBit = TReading & 0x8000;  // test most sig bit
    if (SignBit){
        TReading = (TReading ^ 0xffff) + 1; // 2's comp
    }
    Tc_100 = (6 * TReading) + TReading / 4;    // mal (100 * 0.0625) oder 6.25
    /* Für DS18S20 folgendes verwenden Tc_100 = (TReading*100/2);    */
    return Tc_100/100.0;  // Ganzzahlen und Brüche trennen
}

String Hardware1WireSensor::getValue(){
    byte data[12];
    byte present = 0;
    ds->reset();
    ds->select(addr[0]);
    ds->write(0x44,1);
    delay(1000); 
    present = ds->reset();
    ds->select(addr[0]);    
    ds->write(0xBE);         // Wert lesen
    Serial.print("P=");
    Serial.print(present,HEX);
    Serial.print(" ");
    
    for ( int i = 0; i < 9; i++) {           // 9 bytes
        data[i] = ds->read();
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.print(" CRC=");
    Serial.print( OneWire::crc8( data, 8), HEX);
    Serial.println();
    return String(_getSensorValue(0));
}

String Hardware1WireSensor::getStringState(){
    return String("");
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

String HardwareTele::getStringState(){
    return getValue();
}


HardwareList::HardwareList(){
    _list[0] = new HardwareRelay(16, "Relais 1", "relay1");
    _list[1] = new HardwareRelay(17, "Relais 2", "relay2");
    _list[2] = new HardwareTele("RSSI", "rssi", [](){return String(WiFi.RSSI());}, "WiFi-Signalstaerke");
    _list[3] = new HardwareTele("Temperatur", "temperature", [](){return String((temprature_sens_read()-32)/1.8);}, "WiFi-Signalstaerke");
    _list[4] = new Hardware230VSensor(15, "Sensor 230V 1", "sensor1");
    _list[5] = new Hardware230VSensor( 2, "Sensor 230V 1", "sensor2");
    _list[6] = new Hardware230VSensor( 0, "Sensor 230V 1", "sensor3");
    _list[7] = new Hardware230VSensor( 4, "Sensor 230V 1", "sensor4");
    _list[8] = new Hardware1WireSensor( 5, "OneWire", "onewire");
}

int HardwareList::getLen(){
    return _len;
}

HardwareIO* HardwareList::getElement(int i){
    return _list[i];
}