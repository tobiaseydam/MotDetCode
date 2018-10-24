#include "mqttServer.h"

void mqttServerBuilder::_connectToMqtt() {
    Serial.println("Connecting to MQTT...");
    _mqttClient->connect();
}

void mqttServerBuilder::_connectToWifi() {
    Serial.println("Connecting to Wi-Fi...");
}

void mqttServerBuilder::_onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    uint16_t packetIdSub = _mqttClient->subscribe("test/lol", 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.println(packetIdSub);
    _mqttClient->publish("test/lol", 0, true, "test 1");
    Serial.println("Publishing at QoS 0");
    uint16_t packetIdPub1 = _mqttClient->publish("test/lol", 1, true, "test 2");
    Serial.print("Publishing at QoS 1, packetId: ");
    Serial.println(packetIdPub1);
    uint16_t packetIdPub2 = _mqttClient->publish("test/lol", 2, true, "test 3");
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

void mqttServerBuilder::init(asyncSM* sm){
    _asyncSM = sm;
    _mqttClient->setServer(IPAddress(192, 168, 178, 23), 1883);
    _mqttClient->onConnect(_onMqttConnect);
    _mqttClient->onDisconnect(_onMqttDisconnect);
    _mqttClient->onSubscribe(_onMqttSubscribe);
    _mqttClient->onUnsubscribe(_onMqttUnsubscribe);
    _mqttClient->onMessage(_onMqttMessage);
    _mqttClient->onPublish(_onMqttPublish);
    sm->setMqttServer(_mqttClient);
}
