
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>
#include "Logger.h"
#include "Horn.h"
#include "Settings.h"
#include "StateMachineStates.h"


class StateMachine
{
private:
  Horn* _pHorn;
  Settings* _pSettings;
private:
 EStates _currentState;
 StateMachineState_Base* _pStateBase;
public:
  StateMachine(Horn* pHorn);
public:
  void Setup();
  void Loop();
public:
  bool Reset();
  bool Emergency();
  bool Start(int classFlag, int prepareMinutes, int countdownMinutes);
  bool Abort();
  bool AbortAll();
  bool AbortSingle();
  EStates GetState() { return _currentState; };
public:
  void GetCurrentStateAsJson(String& message);
private:
  void SetState(EStates state);
};

#endif
