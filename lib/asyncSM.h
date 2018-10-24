#ifndef ASYNCSM_H
    #define ASYNCSM_H

    #include "ESPAsyncWebServer.h"
    #include <AsyncMqttClient.h>
    #include <WString.h>

    #define WIFI_FILE       "/wifi.txt"
    #define MQTT_FILE       "/mqtt.txt"
    #define WIFI_TIMEOUT    5
    #define WIFI_AP_SSID    "ESP32"
    #define WIFI_AP_PASS    "MotDet"

    class asyncSM{
        private:
            struct wifiConfig {
                char ssid[64];
                char pass[64];
                bool loaded = false;
            };

            wifiConfig _wifiConfig;

            struct mqttConfig {
                char server[20];
                char user[64];
                char pass[64];
                char devname[64];
                bool loaded = false;
            };

            mqttConfig _mqttConfig;

            enum eSMState {
                START, 

                WIFI_LOOK_FOR_DATA, 
                WIFI_LOGIN,
                WIFI_OPEN_ACCESSPOINT,
                WIFI_WAIT_FOR_CONFIG,

                MQTT_LOOK_FOR_DATA,
                MQTT_LOGIN,

                MAIN_HANDLE_MQTT,

                UNRESOLVABLE_ERROR
                };

            enum eSMRunningState {
                NOT_STARTED, 
                RUNNING, 
                PAUSED, 
                ERROR
                };

            eSMState _state = START;
            eSMRunningState _runningState = NOT_STARTED;

            AsyncWebServer* _httpServer;
            AsyncMqttClient* _mqttServer;

            void _nextStep();
            void _start();
            void _wifiLookForData();
            void _wifiLogin();
            void _wifiOpenAccessPoint();
            void _wifiWaitForConfig();
            void _mqttLookForData();
            void _mqttLogin();
            void _mainHandleMqtt();
            void _loadWifiConfig();
            void _loadMqttConfig();
        public:
            void begin(void *pvParameter);
            void setHttpServer(AsyncWebServer *httpServer);
            void setMqttServer(AsyncMqttClient *mqttServer);
            void setWifiSSID(String ssid);
            void setWifiPass(String pass);
            void saveWifiConfig();
            void setMqttServer(String server);
            void setMqttUser(String user);
            void setMqttPass(String pass);
            void setMqttDevName(String devname);
            void saveMqttConfig();
            String getWifiSSID();
            String getWifiPass();
            String getMqttServer();
            String getMqttUser();
            String getMqttPass();
            String getMqttDevName();

            asyncSM();
    };

#endif