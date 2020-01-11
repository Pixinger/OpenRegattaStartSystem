#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Logger.h"
#include "Horn.h"
#include "StateMachine.h"
#include "NetworkManager.h"


Horn* gpHorn = NULL;
StateMachine* gpStateMachine = NULL;
NetworkManager* gpNetworkManager = NULL;


void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  DEBUG_SERIALWAIT_TIMEOUT(); // wait for serial port to connect. Needed for native USB port only
  DEBUGLOGLN("Regatta Start Automat: Version 1.0");

  gpHorn = new Horn();
  gpStateMachine = new StateMachine(gpHorn);
  gpNetworkManager = new NetworkManager();

  gpHorn->Setup();
  gpStateMachine->Setup();
  gpNetworkManager->Setup(gpStateMachine);
}

#ifdef DEBUGLOG_ENABLED
unsigned long nlt = 0;
#endif

void loop() 
{
  gpHorn->Loop();
  gpStateMachine->Loop(); 
  gpNetworkManager->Loop();

#ifdef DEBUGLOG_ENABLED
  if (nlt < millis())
  {
    nlt = millis() + 1000;
    DEBUGLOG("Main Loop: State="); DEBUGLOGLN(gpStateMachine->GetState());    
  }
#endif
}
