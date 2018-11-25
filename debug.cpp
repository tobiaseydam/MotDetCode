#include "debug.h"

void debug::init(){
    if(DEBUGLVL>0){
        Serial.begin(115200);
        Serial.print("starting serial debugging Level: ");
        Serial.println(DEBUGLVL);
    }
}

void debug::log(String s){
    log(s,1);
}

void debug::log(String s, int dbglvl){
    if((DEBUGLVL>0)&(DEBUGLVL>dbglvl)){
        Serial.print(s);
    }
}

void debug::logln(String s){
    logln(s,1);
}

void debug::logln(byte s){
    logln(String(s),1);
}

void debug::logln(String s, int dbglvl){
    if((DEBUGLVL>0)&(DEBUGLVL>dbglvl)){
        Serial.println(s);
    }
}