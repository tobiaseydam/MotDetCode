#include "tools.h"


void tools::init(){
	if(!SPIFFS.begin(true)){
        debug::logln("SPIFFS mount failed...", 2);
	}else{
        debug::logln("SPIFFS mounted..." , 2);
    }
}

void tools::saveJsonFile(String filename, JsonObject *json){
    debug::log("saving JSON file: ", 2);
    debug::log(filename,2);
    File file = SPIFFS.open(filename, "w");
    json->printTo(file);
    file.close();
    debug::logln(" done", 2);
}

JsonObject& tools::loadJsonFile(String filename){
    File file = SPIFFS.open(filename, "r");
    StaticJsonBuffer<2000> jsonBuffer;
    if(!file){
        return jsonBuffer.createObject();
    }else{
        return jsonBuffer.parseObject(file);
    }
}

bool tools::fileExist(String filename){
    if(!SPIFFS.exists(filename)){
        debug::log(filename, 2);
        debug::logln(" not found", 2);
        return false;
    }
    return true;
}

IPAddress tools::strToIP(String str){
    uint8_t ip[4];
    sscanf(str.c_str(), "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]);
    return IPAddress(ip[0],ip[1],ip[2],ip[3]);
}