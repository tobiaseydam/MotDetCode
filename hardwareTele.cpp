#include "hardwareManager.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();
//uint8_t g_phyFuns;

#ifdef __cplusplus
}
#endif

HardwareTeleRSSI::HardwareTeleRSSI(String name, String mqttFragment2){
    _name = name;
    _mqttFragment0 = "tele";
    _mqttFragment2 = mqttFragment2;
    _type = TELEMETRY;
    _txtDesc = "WiFi-Signalstaerke (RSSI)";
    _constUpdate = true;
}

String HardwareTeleRSSI::getTxtState(){
    return String(WiFi.RSSI());
}

void HardwareTeleRSSI::handleMQTT(String payload){
    return;
}

HardwareTeleTemperature::HardwareTeleTemperature(String name, String mqttFragment2){
    _name = name;
    _mqttFragment0 = "tele";
    _mqttFragment2 = mqttFragment2;
    _type = TELEMETRY;
    _txtDesc = "Prozessortemperatur";
    _constUpdate = true;
}

String HardwareTeleTemperature::getTxtState(){
    return String((temprature_sens_read()-32)/1.8);
}

void HardwareTeleTemperature::handleMQTT(String payload){
    return;
}