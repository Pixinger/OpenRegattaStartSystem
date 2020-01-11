#ifndef LOGGER_H_
#define LOGGER_H_

  #define DEBUGLOG_ENABLED

  #ifdef DEBUGLOG_ENABLED
    #define DEBUG_SERIALWAIT() {while (!Serial) {}}
    #define DEBUG_SERIALWAIT_TIMEOUT() {unsigned long __tick = millis() + 3000; while ((!Serial) && (__tick > millis()))  {}}
    #define DEBUGLOGLN(__x) Serial.println(__x)
    #define DEBUGLOG(__x) Serial.print(__x)
    #define INFOLOGLN(__x) DEBUGLOGLN(__x)
    #define INFOLOG(__x) DEBUGLOG(__x)
    #define WARNLOGLN(__x) DEBUGLOGLN(__x)
    #define WARNLOG(__x)  DEBUGLOG(__x)
    #define ERRORLOGLN(__x) DEBUGLOGLN(__x)
    #define ERRORLOG(__x) DEBUGLOG(__x)
  #else
    #define DEBUG_SERIALWAIT()
    #define DEBUG_SERIALWAIT_TIMEOUT()
    #define DEBUGLOGLN(__x)
    #define DEBUGLOG(__x)
    #define INFOLOGLN(__x) DEBUGLOGLN(__x)
    #define WARNLOGLN(__x) DEBUGLOGLN(__x)
    #define INFOLOG(__x) DEBUGLOG(__x)
    #define WARNLOG(__x) DEBUGLOG(__x)
    #define ERRORLOGLN(__x) DEBUGLOGLN(__x)
    #define ERRORLOG(__x) DEBUGLOG(__x)
  #endif

#endif
