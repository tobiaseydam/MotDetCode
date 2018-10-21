#include "lib/debug.cpp"
#include "lib/tools.cpp"
#include "lib/asyncSM.cpp"
#include "lib/httpServer.cpp"
#include <ArduinoJson.h>

//DynamicJsonBuffer jsonBuffer;




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
	asyncSM sm;
	httpServerBuilder::init(&sm);
	sm.begin(NULL);
	//httpServer.begin();
} 

void loop()
{
	debug::logln("x");
	delay(1000);

}
