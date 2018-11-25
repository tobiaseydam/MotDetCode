#include "mqttServer.h"

void mqttServerBuilder::_connectToMqtt() {
    debug::logln("Connecting to MQTT...");
    asyncSM::getInstance()->getMqttClient()->connect();
}

void mqttServerBuilder::_connectToWifi() {
    debug::logln("Connecting to Wi-Fi...");
}

void mqttServerBuilder::_onMqttConnect(bool sessionPresent) {
    debug::logln("Connected to MQTT.");

    HardwareList* _hwl = asyncSM::getInstance()->_hardware;
    AsyncMqttClient* _m = asyncSM::getInstance()->getMqttClient();
    for(int i = 0; i < _hwl->getLen(); i++){
        HardwareIO* _hio = _hwl->getElement(i);
        if(_hio->getType() == RELAY){
            HardwareRelay* _hr = (HardwareRelay*)_hio;
            _m->subscribe(_hr->getMqttTopic().c_str(), 2);
        }
    }
}

void mqttServerBuilder::_onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    debug::logln("Disconnected from MQTT.");

  //if (WiFi.isConnected()) {
  //  xTimerStart(mqttReconnectTimer, 0);
  //}
}

void mqttServerBuilder::_onMqttSubscribe(uint16_t packetId, uint8_t qos) {

}

void mqttServerBuilder::_onMqttUnsubscribe(uint16_t packetId) {

}

void mqttServerBuilder::_onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    debug::logln("Publish received.");
    debug::log("  topic: ");
    debug::logln(topic);
    debug::log("  qos: ");
    debug::logln(properties.qos);
    debug::log("  dup: ");
    debug::logln(properties.dup);
    debug::log("  retain: ");
    debug::logln(properties.retain);
    debug::log("  len: ");
    debug::logln(len);
    debug::log("  index: ");
    debug::logln(index);
    debug::log("  total: ");
    debug::logln(total);

    HardwareList* _hwl = asyncSM::getInstance()->_hardware;
    AsyncMqttClient* _m = asyncSM::getInstance()->getMqttClient();

    for(int i = 0; i<_hwl->getLen(); i++){
        HardwareIO* _hio = _hwl->getElement(i);
        if(strcmp(_hio->getMqttTopic().c_str(), topic)==0){
            debug::log("Set ");
            debug::log(_hio->getName());
            debug::log(" to ");
            debug::logln(payload);
            if(_hio->getType()==RELAY){
                _hio->handleMQTT(payload);
            }
        }
    }
}

void mqttServerBuilder::_onMqttPublish(uint16_t packetId) {

}

void mqttServerBuilder::init(){
    asyncSM *sm = asyncSM::getInstance();
    AsyncMqttClient *client = sm->getMqttClient();

    IPAddress ip = tools::strToIP(sm->getMqttConfigKey("SERVER"));
    client->setServer(ip, 1883);
    asyncSM::getInstance()->getMqttClient()->onConnect(_onMqttConnect);
    asyncSM::getInstance()->getMqttClient()->onDisconnect(_onMqttDisconnect);
    asyncSM::getInstance()->getMqttClient()->onSubscribe(_onMqttSubscribe);
    asyncSM::getInstance()->getMqttClient()->onUnsubscribe(_onMqttUnsubscribe);
    asyncSM::getInstance()->getMqttClient()->onMessage(_onMqttMessage);
    asyncSM::getInstance()->getMqttClient()->onPublish(_onMqttPublish);
}
