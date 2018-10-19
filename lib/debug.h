#ifndef DEBUG_H
    #define DEBUG_H

    #include <WString.h>

    /*
     *  0   keine Debugausgaben
     *  1   wenig Debugausgaben
     *  ...
     *  3   viele Debugausgaben
     */
    #define DEBUGLVL    3

    class debug{
        public:
            static void init();
            static void log(String s);
            static void log(String s, int dbglvl);
            static void logln(String s);
            static void logln(String s, int dbglvl);
    };

#endif