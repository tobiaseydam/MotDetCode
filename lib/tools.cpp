#include "tools.h"
#include "debug.h"

void tools::init(){
	if(!SPIFFS.begin(true)){
        debug::logln("SPIFFS mount failed...");
	}else{
        debug::logln("SPIFFS mounted...");
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
    debug::log("loading JSON file: ", 2);
    debug::logln(filename,2);
    File file = SPIFFS.open(filename, "r");
    DynamicJsonBuffer jsonBuffer;
    if(!file){
        debug::logln(" not found", 2);
        return jsonBuffer.createObject();
    }else{
        return jsonBuffer.parseObject(file);
        debug::logln(" done", 2);
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
