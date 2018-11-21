#ifndef HARDWAREMANAGER_H
    #define HARDWAREMANAGER_H

    #include <WString.h>
    #include <Arduino.h>
    #include <WiFi.h>
    #include <OneWire.h>

    class HardwareList;

    #include "asyncSM.h"

    enum eHardwareType{
        RELAY,
        INPUT230V,
        TELEMETRY,
        ONEWIRE
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
            virtual String getStringState();
    };

    enum eState{
        ON,
        OFF
    };

    class HardwareRelay: public HardwareIO{
        protected:
            int _pin;
        public: 
            HardwareRelay(int pin, String name, String mqttFragment);
            void setState(eState state);
            void setStringState(char* state);
            eState getState();
            String getStringState();
    };

    class Hardware230VSensor: public HardwareIO{
        protected:
            int _pin;
            boolean changed;
            eState lastState = OFF;
        public: 
            Hardware230VSensor(int pin, String name, String mqttFragment);
            eState getState();
            String getValue();
            String getStringState();
            boolean hasChanged();
            void read();
    };

    class Hardware1WireSensor: public HardwareIO{
        protected:
            int _pin;
            OneWire* ds;
            byte addr[16][8];
            byte numDev = 0;
            float _getSensorValue(byte k);
        public: 
            Hardware1WireSensor(int pin, String name, String mqttFragment);
            String getValue();
            String getStringState();
    };

    class HardwareTele: public HardwareIO{
        private:
            String (*_getVal)();
        public:
            HardwareTele(String name, String mqttFragment, String (*getVal)(), String webDesc);
            String getValue();
            String getStringState();
    };

    class HardwareList{
        protected:
            static const int _len = 9;
            HardwareIO* _list[_len];
        public:
            int getLen();
            HardwareIO* getElement(int i);
            HardwareList();
    };

#endif