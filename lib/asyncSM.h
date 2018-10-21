#ifndef ASYNCSM_H
    #define ASYNCSM_H

    #include "ESPAsyncWebServer.h"
    #include <WString.h>

    #define WIFI_FILE       "/wifi2.txt"
    #define WIFI_TIMEOUT    20
    #define WIFI_AP_SSID    "ESP32"
    #define WIFI_AP_PASS    "MotDet"

    class asyncSM{
        private:
            struct wifiConfig {
                String ssid;
                String pass;
            };

            wifiConfig _wifiConfig;

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
        public:
            void begin(void *pvParameter);
            void setServer(AsyncWebServer httpServer);
            void setWifiSSID(String ssid);
            void setWifiPass(String pass);
            void saveWifiConfig();
            asyncSM();
    };

#endif