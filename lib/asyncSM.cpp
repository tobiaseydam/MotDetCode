#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FS.h"
#include "SPIFFS.h"

#include "Utils.cpp"

#include "asyncSM.h"

void asyncSM::_nextStep(){
    switch(_state){
        case START:  
            _start();
            break;
    }
}

void asyncSM::_start(){
        #ifdef DEBUG
            Serial.begin(115200);
        #endif
        serialLog("entering: sm_start");
        _runningState = RUNNING;
        if(!SPIFFS.begin(true)){
            serialLogln("SPIFFS Mount failed...");
            _state = UNRESOLVABLE_ERROR;
        }
        _state = WIFI_LOOK_FOR_DATA;
}