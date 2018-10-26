#include "mqttServer.h"

void mqttServerBuilder::_connectToMqtt() {
    Serial.println("Connecting to MQTT...");
    asyncSM::getInstance()->getMqttClient()->connect();
}

void mqttServerBuilder::_connectToWifi() {
    Serial.println("Connecting to Wi-Fi...");
}

void mqttServerBuilder::_onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    uint16_t packetIdSub = asyncSM::getInstance()->getMqttClient()->subscribe("test/lol", 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.println(packetIdSub);
    asyncSM::getInstance()->getMqttClient()->publish("test/lol", 0, true, "test 1");
    Serial.println("Publishing at QoS 0");
    uint16_t packetIdPub1 = asyncSM::getInstance()->getMqttClient()->publish("test/lol", 1, true, "test 2");
    Serial.print("Publishing at QoS 1, packetId: ");
    Serial.println(packetIdPub1);
    uint16_t packetIdPub2 = asyncSM::getInstance()->getMqttClient()->publish("test/lol", 2, true, "test 3");
    Serial.print("Publishing at QoS 2, packetId: ");
    Serial.println(packetIdPub2);
}

void mqttServerBuilder::_onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("Disconnected from MQTT.");

  //if (WiFi.isConnected()) {
  //  xTimerStart(mqttReconnectTimer, 0);
  //}
}

void mqttServerBuilder::_onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
}

void mqttServerBuilder::_onMqttUnsubscribe(uint16_t packetId) {
    Serial.println("Unsubscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

void mqttServerBuilder::_onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    Serial.println("Publish received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  qos: ");
    Serial.println(properties.qos);
    Serial.print("  dup: ");
    Serial.println(properties.dup);
    Serial.print("  retain: ");
    Serial.println(properties.retain);
    Serial.print("  len: ");
    Serial.println(len);
    Serial.print("  index: ");
    Serial.println(index);
    Serial.print("  total: ");
    Serial.println(total);
}

void mqttServerBuilder::_onMqttPublish(uint16_t packetId) {
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

void mqttServerBuilder::init(){
    asyncSM *sm = asyncSM::getInstance();
    AsyncMqttClient *client = sm->getMqttClient();

    IPAddress ip = tools::strToIP(sm->getMqttServer());
    client->setServer(ip, 1883);
    asyncSM::getInstance()->getMqttClient()->onConnect(_onMqttConnect);
    asyncSM::getInstance()->getMqttClient()->onDisconnect(_onMqttDisconnect);
    asyncSM::getInstance()->getMqttClient()->onSubscribe(_onMqttSubscribe);
    asyncSM::getInstance()->getMqttClient()->onUnsubscribe(_onMqttUnsubscribe);
    asyncSM::getInstance()->getMqttClient()->onMessage(_onMqttMessage);
    asyncSM::getInstance()->getMqttClient()->onPublish(_onMqttPublish);
}
