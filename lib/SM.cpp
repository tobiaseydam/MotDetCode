#ifndef SM_CPP
    #define SM_CPP
    
    #include "SM_defines.h"
    #include "mainproc.cpp"
    #include "SM_main.cpp"
    #include "SM_wifi.cpp"
    #include "SM_mqtt.cpp"

    static void runSM(){
        switch(sm_state){

            case SM_START:  
                sm_start();
                break;

            case SM_WIFI_LOOK_FOR_DATA:
                sm_wifi_look_for_data();
                break;

            case SM_WIFI_LOGIN:
                sm_wifi_login();
                break;

            case SM_WIFI_CHECK:
                sm_wifi_check();
                break;

            case SM_WIFI_OPEN_ACCESSPOINT:
                sm_wifi_open_accesspoint();
                break;

            case SM_WIFI_WAIT_FOR_CLIENT:
                sm_wifi_wait_for_client();
                break;

            case SM_WIFI_ASK_FOR_SSID:
                sm_wifi_ask_for_ssid();
                break;

            case SM_WIFI_ASK_FOR_PASS:
                sm_wifi_ask_for_pass();
                break;

            case SM_WIFI_SAVE_DATA:
                sm_wifi_save_data();
                break;

            case SM_MQTT_LOOK_FOR_DATA:
                sm_mqtt_look_for_data();
                break;

            case SM_MQTT_LOGIN:
                sm_mqtt_login();
                break;

            case SM_MQTT_CHECK:
                sm_mqtt_check();
                break;

            case SM_MQTT_WAIT_FOR_CLIENT:
                sm_mqtt_wait_for_client();
                break;

            case SM_MQTT_ASK_FOR_SERVER:
                sm_mqtt_ask_for_server();
                break;

            case SM_MQTT_ASK_FOR_NAME:
                sm_mqtt_ask_for_name();
                break;

            case SM_MQTT_ASK_FOR_PASS:
                sm_mqtt_ask_for_pass();
                break;

            case SM_MQTT_ASK_FOR_DEVICE_NAME:
                sm_mqtt_ask_for_device_name();
                break;

            case SM_MQTT_ASK_FOR_TOPICS:
                sm_mqtt_ask_for_topics();
                break;

            case SM_MQTT_SAVE_DATA:
                sm_mqtt_save_data();
                break;

            case SM_MAIN_HANDLE_MQTT:
                sm_main_handle_mqtt();
                break;

            case SM_MAIN_SWITCH_RELAYS:
                sm_main_switch_relays();
                break;

            case SM_MAIN_CHECK_MOTIONS:
                sm_main_check_motions();
                break;

            case SM_MAIN_CHECK_STATE:
                sm_main_check_state();
                break;

            case SM_MAIN_SEND_MQTT:
                sm_main_send_mqtt();
                break;

            case SM_MAIN_CONFIG_ENTER:
                sm_main_config_enter();
                break;

            case SM_MAIN_CONFIG_EDIT_WIFI:
                sm_main_config_edit_wifi();
                break;

            case SM_MAIN_CONFIG_EDIT_MQTT:
                sm_main_config_edit_mqtt();
                break;

            case SM_MAIN_CONFIG_REBOOT:
                sm_main_config_reboot();
                break;

            default:
                //printf("error...");
                break;
        }
    }   

#endif