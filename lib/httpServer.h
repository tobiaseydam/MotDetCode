#ifndef HTTPSERVER_H
    #define HTTPSERVER_H

    #include <WString.h>
    #include "ESPAsyncWebServer.h"

    static AsyncWebServer httpServer(80);

    class httpServerBuilder{
        private:
            static void _handleSPIFFS(AsyncWebServerRequest *request);
            static void _handleUpdate(AsyncWebServerRequest *request);
            static void _handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
            static String _processor(const String &var);
        public:
            static void init();
    };

#endif