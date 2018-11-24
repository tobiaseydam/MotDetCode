#include "hardwareManager.h"

HardwareOneWireSensor::HardwareOneWireSensor(int pin, String name, String mqttFragment2){
    _pin = pin;
    _name = name;
    _mqttFragment0 = "ow";
    _mqttFragment2 = mqttFragment2;
    _type = ONEWIRE;
    _txtDesc = String("OneWire Sensoren, nur lesen");
    _constUpdate = true;
    _ds = new OneWire(_pin);

    // angeschlossene Sensoren suchen
    for(byte i = 0; i<_maxDevs; i++){
        if (!_ds->search(_addr[i])) {
            _numDev = i;
            Serial.print("Keine weiteren Addressen.\n");
            Serial.println(i);
            _ds->reset_search();
            return;
        }
    }
}   

HardwareOneWireSensor::~HardwareOneWireSensor(){
    free(_ds);
}

String HardwareOneWireSensor::getTxtState(){
    String res = "";
    for(int i = 0; i<_numDev; i++){
        res += "Sensor: ";
        res += getAddr(i);
        res += " - Value: ";
        res += String(getSensorValue(i));
        res += "\n";
    }
    return res;
}

byte HardwareOneWireSensor::getNumDevs(){
    return _numDev;
}

String HardwareOneWireSensor::getAddr(byte k){
    String res = "";
    const char* digits = "0123456789ABCDEF";
    for(int i = 0; i < 8; i++){
        if(i>0){
            res = res + " ";
        }
        res = res + digits[(_addr[k][i]&0xF0)>>4] + digits[(_addr[k][i]&0x0F)];
    }
    return res;
}

float HardwareOneWireSensor::getSensorValue(byte k){
    // aktuell nur DS18S20
    byte data[12];
    byte present = 0;
    _ds->reset();
    _ds->select(_addr[k]);
    _ds->write(0x44,1);
    delay(1000); 
    present = _ds->reset();
    _ds->select(_addr[k]);    
    _ds->write(0xBE);
    for ( int i = 0; i < 9; i++) {
        data[i] = _ds->read();
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

void HardwareOneWireSensor::handleMQTT(String payload){
    return;
}