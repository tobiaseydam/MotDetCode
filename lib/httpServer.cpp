#include "httpServer.h"
#include "tools.h"
#include "debug.h"
#include "SPIFFS.h"
#include "FS.h"

void httpServerBuilder::init(){
    httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        debug::logln("neuer Client auf :80");
        request->send(200, "text/plain", "Hello World");
    });

    httpServer.on("/SPIFFS", HTTP_GET, _handleSPIFFS);
    httpServer.on("/UPDATE", HTTP_GET, _handleUpdate);

    httpServer.on("/UPLOAD", HTTP_POST, [](AsyncWebServerRequest * request) {
        request->redirect("/SPIFFS");
    }, _handleUpload);
}

void httpServerBuilder::_handleSPIFFS(AsyncWebServerRequest *request){
    File root = SPIFFS.open("/");

    int cntrParams = request->params();
    if(cntrParams>0){
        AsyncWebParameter *p = request->getParam(0);
        if(p->name().equals("del")){
            SPIFFS.remove(p->value());
        }
        if(p->name().equals("show")){
            request->send(SPIFFS, p->value(), "text/html", false, _processor);
            return;
        }
    }

    if(!root){
        request->send(200, "text/html", "SPIFFS mount failed :(");
    }

    String list = "<html><table>";
    File file = root.openNextFile();
    while(file){
        list += "<tr><td>";
        list += file.name();
        list += "</td><td>";
        list += file.size();
        list += " Bytes</td><td>";
        list += "<a href=\"?del=";
        list += file.name();
        list += "\">delete</a>";
        list += "</td><td>";
        list += "<a href=\"?show=";
        list += file.name();
        list += "\">show</a>";
        list += "</td></tr>";
        file = root.openNextFile();
    }
    list += "</table></html>";
    request->send(200, "text/html", list);
}

void httpServerBuilder::_handleUpdate(AsyncWebServerRequest *request){
    int cntrParams = request->params();
    if(cntrParams>0){
        AsyncWebParameter *p = request->getParam(0);
        if(p->name().equals("type")){
            if(p->value().equals("wifi")){
                AsyncWebParameter *ssid = request->getParam(1);
                AsyncWebParameter *pass = request->getParam(2);
                
                //fileSave_WiFi("/test_wifi.txt", ssid->value().c_str(), pass->value().c_str());
            }else if(p->value().equals("mqtt")){
                AsyncWebParameter *server = request->getParam(1);
                AsyncWebParameter *user = request->getParam(2);
                AsyncWebParameter *pass = request->getParam(3);
                AsyncWebParameter *devname = request->getParam(4);
                
                //fileSave_MQTT("/test_mqtt.txt", server->value().c_str(), user->value().c_str(), pass->value().c_str(), devname->value().c_str());
            }
        }
    }
    request->redirect("/SPIFFS");
}

void httpServerBuilder::_handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
        if (!filename.startsWith("/"))
            filename = "/" + filename;
        if (SPIFFS.exists(filename)) {
            SPIFFS.remove(filename);
        }
        request->_tempFile = SPIFFS.open(filename, "w");
    }
    for (size_t i = 0; i < len; i++) {
        request->_tempFile.write(data[i]);
    }
    if (final) {
        request->_tempFile.close();
    }
}

String httpServerBuilder::_processor(const String &var){
    /*Serial.println(var);
    if (var == "WIFI_SSID"){
        return String(fileParseString(SPIFFS.open(WIFI_FILE, "r"),"ssid"));
    }else if (var == "WIFI_PASS"){
        return String(fileParseString(SPIFFS.open(WIFI_FILE, "r"),"pass"));
    }else if (var == "MQTT_SERVER"){
        return String(fileParseString(SPIFFS.open(MQTT_FILE, "r"),"server"));
    }else if (var == "MQTT_USER"){
        return String(fileParseString(SPIFFS.open(MQTT_FILE, "r"),"user"));
    }else if (var == "MQTT_PASS"){
        return String(fileParseString(SPIFFS.open(MQTT_FILE, "r"),"pass"));
    }else if (var == "MQTT_DEVNAME"){
        return String(fileParseString(SPIFFS.open(MQTT_FILE, "r"),"devName"));
    }*/

    return String();
}