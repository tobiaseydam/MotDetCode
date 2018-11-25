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

    enum eState{
        ON,
        OFF
    };

    // Elternklasse, alle Hardwaregeräte erben hiervon
    class HardwareIO{                       
        protected:
            String _name;                   // Name
            eHardwareType _type;            // Typ
            String _mqttFragment0;          // Fragment 0, vorgegeben durch Typ
            String _mqttFragment2;          // Fragment 1, bei Erstellung vergeben 
            String _txtDesc;                // Beschreibung
            boolean _constUpdate;           // True, wenn regelmäßig MQTT-Nachrichten gesendet werden
        public:
            // Für Ausgabe -------------------
            String getTxtName();            // Name des Geräts, z.B. Interne Prozessor Temperatur oder Relais 1
            String getTxtType();            // Typ des Geräts, z.B. TELEMETRY oder RELAY
            String getTxtDesc();            // Beschreibung, z.B. Temperatur des Prozessors oder Relais, 230V, 8A, 1 Kontakt
            String getMqttFragment(byte i); // MQTT-Fragment an i-ter Stelle, z.B. 
                                            //      0 = tele,stat,... 
                                            //      1 = ESP32_test, ...
                                            //      2 = processor_temperature, relay1, ...
            String getMqttTopic();          // komplettes MQTT-Topic, z.B. tele/ESP32_test/relay1
            boolean isConstUpdate();        // siehe _constUpdate
            virtual String getTxtState();   // Aktueller Zustand oder Messwert
            virtual void handleMQTT(String payload);
                                            // MQTT-Payload in Zustand umsetzen
            // -------------------------------
            String getName();               // Name des Geräts, unformatiert
            eHardwareType getType();        // Typ des Geräts als Enum
    };

    // Relais, erbt von HardwareIO
    class HardwareRelay: public HardwareIO{ 
        protected:
            int _pin;                       // Pin, an den das Relais angeschlossen ist (am ESP32)
        public: 
            void setState(eState state);    // Ein- oder Ausschalten
            void toggle();                  // Aktuellen Zustand umschalten
            void handleMQTT(String payload) override;
                                            // Überschreiben der Elternfunktion
            eState getState();              // aktuelle Stellung lesen
            String getTxtState() override;  // aktuelle Stellung lesen und als Text zurückgeben
    };

    // Relais, OMRON G5LE14, 230V 10A
    class HardwareRelayOmronG5LE14: public HardwareRelay{
        public:
            HardwareRelayOmronG5LE14(int pin, String name, String mqttFragment2);
                                            // Konstruktor
    };

    // Input (Analog oder Digital), erbt von HardwareIO
    class HardwareSensorPin: public HardwareIO{
        protected:
            int _pin;                       // Pin, an dem der Sensor angeschlossen ist (am ESP32)
    };

    // digitaler Input, erbt von HardwareSensorPin
    class HardwareDigitalSensorPin: public HardwareSensorPin{
        protected:
            boolean _changed = false;       // true, wenn sich der Status seit dem letzten auslesen geändert hat
            eState _lastState = OFF;        // letzter gelesener Zustand
        public: 
            void read();                   // aktuelen Wert lesen und _changed setzen
            eState getState();              // aktuellen Zustand lesen
            String getTxtState();           // aktuellen Zustand lesen und als Text zurückgeben
            boolean hasChanged();           // siehe _changed
            void handleMQTT(String payload);// macht nichts
    };


    // 230V Input, z.B. für Bewegungsmelder, erbt von HardwareIO
    class Hardware230VSensor: public HardwareDigitalSensorPin{
        public: 
            Hardware230VSensor(int pin, String name, String mqttFragment2);
                                            // Konstruktor
    };

    // OneWire-Anschluss, z.B. für DS18B20-Sensoren
    class HardwareOneWireSensor: public HardwareIO{
        protected:
            int _pin;                       // Pin, an dem der Bus hängt
            OneWire* _ds;                   // OneWire-Objekt für Zugriff
            static const byte _maxDevs = 16;      // Maximale Anzahl angeschlossener Sensoren
            byte _addr[_maxDevs][8];        // Array mit Adressen der Sensoren
            byte _numDev = 0;               // Anzahl der angeschlossenen Sensoren
        public: 
            HardwareOneWireSensor(int pin, String name, String mqttFragment2);
                                            // Konstruktor
            ~HardwareOneWireSensor();       // Destructor
            String getTxtState();           // Gibt die Adressen und Werte der angeschlossenen Sensoren zurück
            byte getNumDevs();              // Gibt die Anzahl der angeschlossenen Sensoren zurück
            String getAddr(byte k);         // Gibt die Adresse des i-ten Sensors Zurück
            float getSensorValue(byte k);   // Gibt den Wert des i-ten Sensors Zurück
            void handleMQTT(String payload);// macht nichts
            void publishSensors();          // Alle Sensordaten senden
    };

    // Eigene Funktionen des ESP32, z.B. RSSI oder Temperatur
    class HardwareTele: public HardwareIO{
            byte dummy = 0;
    };

    class HardwareTeleRSSI: public HardwareTele{
        public:
            HardwareTeleRSSI(String name, String mqttFragment2);
                                            // Konstruktor
            String getTxtState();           // gibt aktuellen Wert zurück
            void handleMQTT(String payload);// macht nichts
    };

    class HardwareTeleTemperature: public HardwareTele{
        public:
            HardwareTeleTemperature(String name, String mqttFragment2);
                                            // Konstruktor
            String getTxtState();           // gibt aktuellen Wert zurück
            void handleMQTT(String payload);// macht nichts
    };

    // Liste der verfügbaren Hardware
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