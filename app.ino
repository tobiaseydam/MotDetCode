#include "debug.h"
#include "tools.h"
#include "asyncSM.h"
#include "httpServer.h"
#include "mqttServer.h"
#include <ArduinoJson.h>

void setup()
{
	asyncSM* _asyncSM = asyncSM::getInstance();
	httpServerBuilder::init();
	mqttServerBuilder::init();
	_asyncSM->begin(NULL);
} 

void loop()
{
	//debug::logln("x");
	//delay(100);

}
