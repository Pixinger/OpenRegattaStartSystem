#include "StateMachine.h"

#ifndef SAFEDELETE
  #define SAFEDELETE(__x) if (__x != NULL) { delete __x; __x = NULL; }
#endif

StateMachine::StateMachine(Horn* pHorn)
  :_currentState(EStates::Off)
  ,_pHorn(pHorn)
  ,_pSettings(NULL)
  ,_pStateBase(NULL)
{
}

void StateMachine::Setup()
{
  _pSettings = Settings::Load();
  SetState(EStates::Setup);
}

void StateMachine::Loop()
{
  if (_pStateBase != NULL)
  {
    EStates nextState = _pStateBase->Loop();
    if (nextState != _currentState) // Eigentlich überflüssig
    {
      //DEBUGLOG("StateMachine Loop2: switching state from "); DEBUGLOG(_currentState); DEBUGLOG(" to "); DEBUGLOGLN(nextState);
      SetState(nextState);      
    }
  }
  else
  {
    //DEBUGLOGLN("StateMachine::Loop:0 NULL");    
  }
}

bool StateMachine::Reset()
{
  DEBUGLOGLN("StateMachine::Reset()");
  _pHorn->Off();
  SetState(EStates::Setup);
  return true;
}

bool StateMachine::Emergency()
{
  DEBUGLOGLN("StateMachine::Emergency()");
  _pHorn->Off();
  SetState(EStates::Off);
  return true;
}

bool StateMachine::Start(int classFlag, int prepareMinutes, int countdownMinutes)
{
  DEBUGLOGLN("StateMachine::Start()");
  // Check if Start is allowed in this mode:
  if (_currentState != EStates::Setup)
  {
    WARNLOGLN("Error: Not allowed in state: "); DEBUGLOGLN(_currentState);
    return false;
  }

  // Update Settings
  _pSettings->setClassFlag(classFlag);
    DEBUGLOG("_pSettings->setClassFlag: "); DEBUGLOGLN(classFlag);
  _pSettings->setPrepareMinutes(prepareMinutes);
    DEBUGLOG("_pSettings->setPrepareMinutes: "); DEBUGLOGLN(prepareMinutes);
  _pSettings->setCountdownMinutes(countdownMinutes);
    DEBUGLOG("_pSettings->setCountdownMinutes: "); DEBUGLOGLN(countdownMinutes);

  SetState(EStates::Prepare);
  return true;
}

bool StateMachine::Abort()
{
  DEBUGLOGLN("StateMachine::Abort()");
  if ((_currentState == EStates::Prepare) || (_currentState == EStates::Countdown4) || (_currentState == EStates::Countdown1))
  {
    SetState(EStates::Aborted);
    return true;
  }
  else
  {
    WARNLOGLN("Error: Not allowed in state: "); DEBUGLOGLN(_currentState);
    return false;
  }
}

bool StateMachine::AbortAll()
{
  DEBUGLOGLN("StateMachine::AbortAll()");
  if (_currentState == EStates::Started)
  {
    SetState(EStates::StartedAbortedAll);
    return true;
  }
  else
  {
    WARNLOGLN("Error: Not allowed in state: "); DEBUGLOGLN(_currentState);
    return false;
  }
}

bool StateMachine::AbortSingle()
{
  DEBUGLOGLN("StateMachine::AbortSingle()");
  if (_currentState == EStates::Started)
  {
    SetState(EStates::StartedAbortedSingle);
    return true;
  }
  else
  {
    WARNLOGLN("Error: Not allowed in state: "); DEBUGLOGLN(_currentState);
    return false;
  }
}

void StateMachine::GetCurrentStateAsJson(String& message)
{
  message += "\"ver\" : \"1\",";
  if (_pStateBase != NULL)
  {
	  _pStateBase->GetStateAsJson(message);
  }
  else
  {   
    message += "\"state\" : \"Unknown\"";
  }
}

void StateMachine::SetState(EStates state)
{
  if (_currentState == state)
    return;

  DEBUGLOG("StateMachine::SetState1: switching state from "); DEBUGLOG(_currentState); DEBUGLOG(" to "); DEBUGLOGLN(state);
  if (_pStateBase != NULL)
  {
    delete _pStateBase;
    _pStateBase = NULL;
  }

  _currentState = state;
  switch(state)
  {
    case EStates::Setup:
      DEBUGLOGLN("SetState Setup");
      _pStateBase = new StateMachineState_Setup(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    case EStates::Prepare:
      DEBUGLOGLN("SetState Prepare");
      _pStateBase = new StateMachineState_Prepare(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    case EStates::Countdown4:
      DEBUGLOGLN("SetState Countdown4");
      _pStateBase = new StateMachineState_Countdown4(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    case EStates::Countdown1:
      DEBUGLOGLN("SetState Countown1");
      _pStateBase = new StateMachineState_Countdown1(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    case EStates::Started:
      DEBUGLOGLN("SetState Started");
      _pStateBase = new StateMachineState_Started(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    case EStates::StartedAbortedAll:
      DEBUGLOGLN("SetState AbAll");
      _pStateBase = new StateMachineState_StartedAbortedAll(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    case EStates::StartedAbortedSingle:
      DEBUGLOGLN("SetState AbSin");
      _pStateBase = new StateMachineState_StartedAbortedSingle(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    case EStates::Aborted:
      DEBUGLOGLN("SetState Aborted");
      _pStateBase = new StateMachineState_Aborted(_pSettings, _pHorn);
      _pStateBase->Started();
      break;
    default:
      DEBUGLOGLN("SetState DEFAULT");    
      break;
  }
}
