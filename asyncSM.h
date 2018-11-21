#ifndef ASYNCSM_H
    #define ASYNCSM_H

    #include "ESPAsyncWebServer.h"
    #include <AsyncMqttClient.h>
    #include <WString.h>
    #include <Ticker.h>
    #include "debug.h"
    #include "hardwareManager.h"

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
            AsyncMqttClient* _mqttClient;
            

            Ticker* _mqttTimer = new Ticker();
            Ticker* _mqttTimer2 = new Ticker();

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
            static void _handleTeleHardware();
            static void _handleSensorHardware();

            static asyncSM* asyncSMInstance;
        public:
            HardwareList* _hardware;
            static asyncSM* getInstance();
            
            void begin(void *pvParameter);
            
            void setHttpServer(AsyncWebServer *httpServer);
            AsyncWebServer *getHttpServer();
            
            void setMqttClient(AsyncMqttClient *mqttClient);
            AsyncMqttClient *getMqttClient();
            
            void saveWifiConfig();
            void saveMqttConfig();
            
            void setWifiSSID(String ssid);
            void setWifiPass(String pass);
            void setMqttServer(String server);
            void setMqttUser(String user);
            void setMqttPass(String pass);
            void setMqttDevName(String devname);
            
            String getWifiSSID();
            String getWifiPass();
            String getMqttServer();
            String getMqttUser();
            String getMqttPass();
            String getMqttDevName();
            String getMqttTimerIntervall();
            int getMqttTimerIntervallSeconds();
            Ticker* getMqttTimer();

            String getHardwareInfo();
            String getWebHardwareInfo();

            asyncSM();
    };

#endif