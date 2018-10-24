#include "lib/debug.cpp"
#include "lib/tools.cpp"
#include "lib/asyncSM.cpp"
#include "lib/httpServer.cpp"
#include <ArduinoJson.h>
#include "lib/mqttServer.cpp"




void setup()
{
	
	/*debug::init();
	tools::init();
	String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  	JsonObject& root = jsonBuffer.parseObject(input);
	tools::saveJsonFile("/test.txt", &root);

	JsonObject& foo = tools::loadJsonFile("/test.txt");
	foo.printTo(Serial);*/


	asyncSM *sm = new asyncSM();
	httpServerBuilder::init(sm);
	mqttServerBuilder::init(sm);
	sm->begin(NULL);
	delay(5000);
} 

void loop()
{
	//debug::logln("x");
	//delay(100);

}
