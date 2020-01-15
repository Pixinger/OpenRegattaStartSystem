#include "StateMachineStates.h"


// #############################################################
// Setup
// #############################################################
void StateMachineState_Setup::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Off();
}
EStates StateMachineState_Setup::OnLoop()
{
  return EStates::Setup;
}
void StateMachineState_Setup::OnGetStateAsJson(String& message)
{
  DEBUGLOGLN("StateMachineState_Setup: ");
  message += "\"state\" : \"Setup\",";
  message += "\"cfid\" : \"";  message += _pSettings->getClassFlag();  message += "\",";
  message += "\"pmin\" : \"";  message += _pSettings->getPrepareMinutes();  message += "\",";
  message += "\"cmin\" : \"";  message += _pSettings->getCountdownMinutes();  message += "\"";
}


// #############################################################
// Prepare
// #############################################################
void StateMachineState_Prepare::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Short(1);
  _stopMillis = millis() + (_pSettings->getPrepareMinutes() * 1000 * 60);
}
EStates StateMachineState_Prepare::OnLoop()
{
  if (millis() < _stopMillis)
    return EStates::Prepare;
  else
    return EStates::Countdown4;
}
void StateMachineState_Prepare::OnGetStateAsJson(String& message)
{
  message += "\"state\" : \"Prepare\",";
  message += "\"cfid\" : \"";  message += _pSettings->getClassFlag();  message += "\",";

  unsigned long ms = millis();
  if (ms < _stopMillis)
  {
    unsigned long remainingTimeMillis = _stopMillis - ms;
    float remainingTimeSeconds = remainingTimeMillis / 1000.0f;
    message += "\"time\" : \"";
    message += remainingTimeSeconds;
    message += "\"";    
  }
  else
  {
    message += "\"time\" : \"0\"";
  }
}


// #############################################################
// Countdown4
// #############################################################
void StateMachineState_Countdown4::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Short(1);
  _stopMillis = millis() + (_pSettings->getCountdownMinutes() * 1000*60) - (1000*60);
}
EStates StateMachineState_Countdown4::OnLoop()
{
  if (millis() < _stopMillis)
    return EStates::Countdown4;
  else
    return EStates::Countdown1;
}
void StateMachineState_Countdown4::OnGetStateAsJson(String& message)
{
  message += "\"state\" : \"Countdown4\",";
  message += "\"cfid\" : \"";  message += _pSettings->getClassFlag();  message += "\",";

  unsigned long ms = millis();
  if (ms < _stopMillis)
  {
    unsigned long remainingTimeMillis = _stopMillis - ms;
    float remainingTimeSeconds = remainingTimeMillis / 1000.0f;
    message += "\"time\" : \"";
    message += remainingTimeSeconds;
    message += "\"";
  }
  else
  {
    message += "\"time\" : \"0\"";
  }
}


// #############################################################
// Countdown1
// #############################################################
void StateMachineState_Countdown1::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Long(1);
  _stopMillis = millis() + (1000 * 60); // Allways 1 minute!
}
EStates StateMachineState_Countdown1::OnLoop()
{
  if (millis() < _stopMillis)
    return EStates::Countdown1;
  else
    return EStates::Started;
}
void StateMachineState_Countdown1::OnGetStateAsJson(String& message)
{
  message += "\"state\" : \"Countdown1\",";
  message += "\"cfid\" : \"";  message += _pSettings->getClassFlag();  message += "\",";

  unsigned long ms = millis();
  if (ms < _stopMillis)
  {
    unsigned long remainingTimeMillis = _stopMillis - ms;
    float remainingTimeSeconds = remainingTimeMillis / 1000.0f;
    message += "\"time\" : \"";
    message += remainingTimeSeconds;
    message += "\"";
  }
  else
  {
    message += "\"time\" : \"0\"";
  }
}


// #############################################################
// Started
// #############################################################
void StateMachineState_Started::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Short(1);
  _stopMillis = millis() + (1000*60); // Allways 1 minute.
}
EStates StateMachineState_Started::OnLoop()
{
  if (millis() < _stopMillis)
    return EStates::Started;
  else
    return EStates::Setup;
}
void StateMachineState_Started::OnGetStateAsJson(String& message)
{
  message += "\"state\" : \"Started\",";
  unsigned long ms = millis();
  if (ms < _stopMillis)
  {
    unsigned long remainingTimeMillis = _stopMillis - ms;
    float remainingTimeSeconds = remainingTimeMillis / 1000.0f;
    message += "\"time\" : \"";
    message += remainingTimeSeconds;
    message += "\"";
  }
  else
  {
    message += "\"time\" : \"0\"";
  }
}


// #############################################################
// StartedAbortedAll
// #############################################################
void StateMachineState_StartedAbortedAll::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Short(2);
  _stopMillis = millis() + (1000*60); // Allways 1 minute.
}
EStates StateMachineState_StartedAbortedAll::OnLoop()
{
  if (millis() < _stopMillis)
    return EStates::StartedAbortedAll;
  else
    return EStates::Setup;
}
void StateMachineState_StartedAbortedAll::OnGetStateAsJson(String& message)
{
  message += "\"state\" : \"AbortedAll\",";
  unsigned long ms = millis();
  if (ms < _stopMillis)
  {
    unsigned long remainingTimeMillis = _stopMillis - ms;
    float remainingTimeSeconds = remainingTimeMillis / 1000.0f;
    message += "\"time\" : \"";
    message += remainingTimeSeconds;
    message += "\"";
  }
  else
  {
    message += "\"time\" : \"0\"";
  }
}


// #############################################################
// StartedAbortedSingle
// #############################################################
void StateMachineState_StartedAbortedSingle::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Long(1);
  _stopMillis = millis() + (1000*60); // Allways 1 minute.
}
EStates StateMachineState_StartedAbortedSingle::OnLoop()
{
  if (millis() < _stopMillis)
    return EStates::StartedAbortedSingle;
  else
    return EStates::Setup;
}
void StateMachineState_StartedAbortedSingle::OnGetStateAsJson(String& message)
{
  message += "\"state\" : \"AbortedSingle\",";
  unsigned long ms = millis();
  if (ms < _stopMillis)
  {
    unsigned long remainingTimeMillis = _stopMillis - ms;
    float remainingTimeSeconds = remainingTimeMillis / 1000.0f;
    message += "\"time\" : \"";
    message += remainingTimeSeconds;
    message += "\"";
  }
  else
  {
    message += "\"time\" : \"0\"";
  }
}


// #############################################################
// Aborted
// #############################################################
void StateMachineState_Aborted::OnStarted()
{
  StateMachineState_Base::OnStarted();
  _pHorn->Short(3);
  _stopMillis = millis() + (1000*60); // Allways 1 minute.
}
EStates StateMachineState_Aborted::OnLoop()
{
  if (millis() < _stopMillis)
    return EStates::Aborted;
  else
    return EStates::Setup;
}
void StateMachineState_Aborted::OnGetStateAsJson(String& message)
{
  message += "\"state\" : \"Aborted\",";
  unsigned long ms = millis();
  if (ms < _stopMillis)
  {
    unsigned long remainingTimeMillis = _stopMillis - ms;
    float remainingTimeSeconds = remainingTimeMillis / 1000.0f;
    message += "\"time\" : \"";
    message += remainingTimeSeconds;
    message += "\"";
  }
  else
  {
    message += "\"time\" : \"0\"";
  }
}
