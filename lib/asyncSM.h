#ifndef SM_DEFINES_H
    #define SM_DEFINES_H

    class asyncSM{
        private:
            enum eSMState {START, WIFI_LOOK_FOR_DATA, UNRESOLVABLE_ERROR};
            enum eSMRunningState {NOT_STARTED, RUNNING, PAUSED, ERROR};
            eSMState _state = START;
            eSMRunningState _runningState = NOT_STARTED;
            void _nextStep();
            void _start();
        public:
            void begin(void *pvParameter);
    };

#endif