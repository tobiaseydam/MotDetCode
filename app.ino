#include "debug.h"
#include "tools.h"
#include "asyncSM.h"
#include "httpServer.h"
#include "mqttServer.h"
#include <ArduinoJson.h>

HardwareList* _hwl; 

void setup()
{
	asyncSM* _asyncSM = asyncSM::getInstance();
	httpServerBuilder::init();
	mqttServerBuilder::init();
	_asyncSM->begin(NULL);
	HardwareList* _hwl = asyncSM::getInstance()->_hardware;
} 

void loop()
{ 
 
}
