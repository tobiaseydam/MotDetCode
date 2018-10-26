#include "httpServer.h"
#include "tools.h"
#include "debug.h"
#include "SPIFFS.h"
#include "FS.h"

void httpServerBuilder::init(){
    AsyncWebServer *httpServer = asyncSM::getInstance()->getHttpServer();
    httpServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        debug::logln("neuer Client auf :80");
        request->send(200, "text/plain", "Hello World");
    });
    
    httpServer->on("/SPIFFS", HTTP_GET, _handleSPIFFS);
    httpServer->on("/UPDATE", HTTP_GET, _handleUpdate);
    
    httpServer->on("/UPLOAD", HTTP_POST, [](AsyncWebServerRequest * request) {
        request->redirect("/SPIFFS");
    }, _handleUpload);
    //_asyncSM = sm;
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
                asyncSM::getInstance()->setWifiSSID(ssid->value());
                asyncSM::getInstance()->setWifiPass(pass->value());
                asyncSM::getInstance()->saveWifiConfig();
                request->redirect("/SPIFFS");
                ESP.restart();
            }else if(p->value().equals("mqtt")){
                AsyncWebParameter *server = request->getParam(1);
                AsyncWebParameter *user = request->getParam(2);
                AsyncWebParameter *pass = request->getParam(3);
                AsyncWebParameter *devname = request->getParam(4);
                asyncSM::getInstance()->setMqttServer(server->value());
                asyncSM::getInstance()->setMqttUser(user->value());
                asyncSM::getInstance()->setMqttPass(pass->value());
                asyncSM::getInstance()->setMqttDevName(devname->value());
                asyncSM::getInstance()->saveMqttConfig();
                request->redirect("/SPIFFS");
                ESP.restart();
            }
        }
    }
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
    if (var == "WIFI_SSID"){
        return asyncSM::getInstance()->getWifiSSID();
    }else if (var == "WIFI_PASS"){
        return asyncSM::getInstance()->getWifiPass();
    }else if (var == "MQTT_SERVER"){
        return asyncSM::getInstance()->getMqttServer();
    }else if (var == "MQTT_USER"){
        return asyncSM::getInstance()->getMqttUser();
    }else if (var == "MQTT_PASS"){
        return asyncSM::getInstance()->getMqttPass();
    }else if (var == "MQTT_DEVNAME"){
        return asyncSM::getInstance()->getMqttDevName();
    }else if (var == "HARDWAREINFO"){
        return asyncSM::getInstance()->getWebHardwareInfo();
    }

    return String();
}