#ifndef MQTTSERVER_H
    #define MQTTSERVER_H

    #include <AsyncMqttClient.h>
    #include "debug.h"
    #include "asyncSM.h"
    #include "tools.h"
    
    class mqttServerBuilder{
        private:
            TimerHandle_t mqttReconnectTimer;
            TimerHandle_t wifiReconnectTimer;
            static void _connectToMqtt();
            static void _connectToWifi();
            static void _onMqttConnect(bool sessionPresent);
            static void _onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
            static void _onMqttSubscribe(uint16_t packetId, uint8_t qos);
            static void _onMqttUnsubscribe(uint16_t packetId);
            static void _onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
            static void _onMqttPublish(uint16_t packetId);

        public:
            static void init();
    };

#endif