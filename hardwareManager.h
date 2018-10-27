#ifndef HARDWAREMANAGER_H
    #define HARDWAREMANAGER_H

    #include <WString.h>
    #include <Arduino.h>
    #include <WiFi.h>

    class HardwareList;

    #include "asyncSM.h"

    enum eHardwareType{
        RELAY,
        INPUT230V,
        TELEMETRY
    };

    class HardwareIO{
        protected:
            String _name;
            eHardwareType _type;
            String _mqttFragment;
            String _webDesc;
        public:
            String getName();
            eHardwareType getType();
            String getMqttFragment();
            String getMqttTopic();
            String getWebDesc();
    };

    enum eRelayState{
        ON,
        OFF
    };

    class HardwareRelay: public HardwareIO{
        protected:
            int _pin;
        public: 
            HardwareRelay(int pin, String name, String mqttFragment);
            void setState(eRelayState state);
            eRelayState getState();
    };

    class HardwareTele: public HardwareIO{
        private:
            String (*_getVal)();
        public:
            HardwareTele(String name, String mqttFragment, String (*getVal)(), String webDesc);
            String getValue();
    };

    class HardwareList{
        protected:
            static const int _len = 4;
            HardwareIO* _list[_len];
        public:
            int getLen();
            HardwareIO* getElement(int i);

            HardwareList();
    };

#endif