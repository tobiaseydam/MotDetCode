#ifndef HTTPSPIFFS_CPP
    #define HTTPSPIFFS_CPP
        
    #include "SM_defines.h"
    #include "Utils.cpp"
    #include "mainproc.cpp"

    #include "FS.h"
    #include <SPIFFS.h>

    void handleSPIFFS(AsyncWebServerRequest *request){
        File root = SPIFFS.open("/");

        int cntrParams = request->params();
        if(cntrParams>0){
            AsyncWebParameter *p = request->getParam(0);
            if(p->name().equals("del")){
                SPIFFS.remove(p->value());
            }
            if(p->name().equals("show")){
                request->send(SPIFFS, p->value());
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

    void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!index) {
            Serial.printf("UploadStart: %s\n", filename.c_str());
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
            Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
        }
    }


#endif