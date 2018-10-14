#ifndef SM_DEFINES_H
    #define SM_DEFINES_H

    #define NODEBUG

    static void runSM(void);

    #define SM_START                        00

    static void sm_start(void);


    #define SM_WIFI_LOOK_FOR_DATA           01
    #define SM_WIFI_LOGIN                   02
    #define SM_WIFI_CHECK                   03

    static void sm_wifi_look_for_data(void);
    static void sm_wifi_login(void);
    static void sm_wifi_check(void);


    #define SM_WIFI_OPEN_ACCESSPOINT        10
    #define SM_WIFI_WAIT_FOR_CLIENT         11
    #define SM_WIFI_ASK_FOR_SSID            12
    #define SM_WIFI_ASK_FOR_PASS            13
    #define SM_WIFI_SAVE_DATA               14

    static void sm_wifi_open_accesspoint(void);
    static void sm_wifi_wait_for_client(void);
    static void sm_wifi_ask_for_ssid(void);
    static void sm_wifi_ask_for_pass(void);
    static void sm_wifi_save_data(void);


    #define SM_MQTT_LOOK_FOR_DATA           04
    #define SM_MQTT_LOGIN                   05
    #define SM_MQTT_CHECK                   06

    static void sm_mqtt_look_for_data(void);
    static void sm_mqtt_login(void);
    static void sm_mqtt_check(void);


    #define SM_MQTT_WAIT_FOR_CLIENT         20
    #define SM_MQTT_ASK_FOR_SERVER          21
    #define SM_MQTT_ASK_FOR_NAME            22
    #define SM_MQTT_ASK_FOR_PASS            23
    #define SM_MQTT_ASK_FOR_DEVICE_NAME     24
    #define SM_MQTT_ASK_FOR_TOPICS          25
    #define SM_MQTT_SAVE_DATA               26

    static void sm_mqtt_wait_for_client(void);
    static void sm_mqtt_ask_for_server(void);
    static void sm_mqtt_ask_for_name(void);
    static void sm_mqtt_ask_for_pass(void);
    static void sm_mqtt_ask_for_device_name(void);
    static void sm_mqtt_ask_for_topics(void);
    static void sm_mqtt_save_data(void);


    #define SM_MAIN_HANDLE_MQTT             30
    #define SM_MAIN_SWITCH_RELAYS           31
    #define SM_MAIN_CHECK_MOTIONS           32
    #define SM_MAIN_CHECK_STATE             33
    #define SM_MAIN_SEND_MQTT               34

    static void sm_main_handle_mqtt(void);
    static void sm_main_switch_relays(void);
    static void sm_main_check_motions(void);
    static void sm_main_check_state(void);
    static void sm_main_send_mqtt(void);


    #define SM_MAIN_CONFIG_ENTER            40
    #define SM_MAIN_CONFIG_EDIT_WIFI        41
    #define SM_MAIN_CONFIG_EDIT_MQTT        42
    #define SM_MAIN_CONFIG_REBOOT           43

    static void sm_main_config_enter(void);
    static void sm_main_config_edit_wifi(void);
    static void sm_main_config_edit_mqtt(void);
    static void sm_main_config_reboot(void);


    #define SM_UNRESOLVABLE_ERROR           99

    #define WIFI_FILE                       "/wifi.txt"
    #define WIFI_AP_SSID                    "ESP32"
    #define WIFI_AP_PASS                    "MotDet"
    #define WIFI_TIMEOUT                    20

    #define MQTT_FILE                       "/mqtt.txt"

    static int sm_state = SM_START;

    #include "WiFi.h"
    #include "PubSubClient.h"

    static WiFiServer telnetServer(23);
    static WiFiClient mqttWiFiClient;
    static PubSubClient mqttClient(mqttWiFiClient);

    static int sensor_states[] = {-1,-1,-1,-1};

    #define IO_REL_0                    16
    #define IO_REL_1                    17

    #define IO_MODET_0                  15
    #define IO_MODET_1                  2
    #define IO_MODET_2                  0
    #define IO_MODET_3                  4


    #include "SM.cpp"

#endif