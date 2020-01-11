
#ifndef STATEMACHINESTATES_H
#define STATEMACHINESTATES_H

#include <Arduino.h>
#include "Logger.h"
#include "Settings.h"
#include "Horn.h"

enum EStates
{
  Off, // Will not react to network
  Setup,
  Prepare,
  Countdown4,
  Countdown1,
  Started,
  StartedAbortedSingle,
  StartedAbortedAll,
  Aborted
};

// #############################
// Base
// #############################
class StateMachineState_Base
{
protected:
  Horn* _pHorn;
  Settings* _pSettings;
public:
  StateMachineState_Base(Settings* pSettings, Horn* pHorn):_pSettings(pSettings),_pHorn(pHorn) {};
  ~StateMachineState_Base() {};
  void Started() { OnStarted(); }
  EStates Loop() { return OnLoop(); }
	void GetStateAsJson(String& message) { OnGetStateAsJson(message);};
protected:
  virtual void OnStarted() {};
	virtual EStates OnLoop() = 0;
	virtual void OnGetStateAsJson(String& message) = 0;
};

// #############################
// Setup 
// #############################
class StateMachineState_Setup: public StateMachineState_Base
{
public:
  StateMachineState_Setup(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) { DEBUGLOGLN("CTOR: Setup: ");};
protected:
  virtual void OnStarted();
  virtual EStates OnLoop();
	virtual void OnGetStateAsJson(String& message);
};

// #############################
// Prepare 
// #############################
class StateMachineState_Prepare: public StateMachineState_Base
{
private:
  unsigned long _stopMillis;
public:
  StateMachineState_Prepare(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) {DEBUGLOGLN("CTOR: Prepare: ");};
protected:
  virtual void OnStarted();
	virtual EStates OnLoop();
	virtual void OnGetStateAsJson(String& message);
};

// #############################
// Countdown4 
// #############################
class StateMachineState_Countdown4: public StateMachineState_Base
{
private:
  unsigned long _stopMillis;
public:
  StateMachineState_Countdown4(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) {DEBUGLOGLN("CTOR: Countdown4: ");};
protected:
  virtual void OnStarted();
	virtual EStates OnLoop();
	virtual void OnGetStateAsJson(String& message);
};

// #############################
// Countdown1 
// #############################
class StateMachineState_Countdown1: public StateMachineState_Base
{
private:
  unsigned long _stopMillis;
public:
  StateMachineState_Countdown1(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) {DEBUGLOGLN("CTOR: Countdown1: ");};
protected:
  virtual void OnStarted();
	virtual EStates OnLoop();
	virtual void OnGetStateAsJson(String& message);
};

// #############################
// Started 
// #############################
class StateMachineState_Started: public StateMachineState_Base
{
private:
  unsigned long _stopMillis;
public:
  StateMachineState_Started(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) {DEBUGLOGLN("CTOR: Started: ");};
protected:
  virtual void OnStarted();
	virtual EStates OnLoop();
	virtual void OnGetStateAsJson(String& message);
};

// #############################
// StartedAbortedAll 
// #############################
class StateMachineState_StartedAbortedAll: public StateMachineState_Base
{
private:
  unsigned long _stopMillis;
public:
  StateMachineState_StartedAbortedAll(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) {DEBUGLOGLN("CTOR: AvortedAll: ");};
protected:
  virtual void OnStarted();
	virtual EStates OnLoop();
	virtual void OnGetStateAsJson(String& message);
};

// #############################
// StartedAbortedSingle 
// #############################
class StateMachineState_StartedAbortedSingle: public StateMachineState_Base
{
private:
  unsigned long _stopMillis;
public:
  StateMachineState_StartedAbortedSingle(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) {DEBUGLOGLN("CTOR: AbortedSingle: ");};
protected:
  virtual void OnStarted();
  virtual EStates OnLoop();
  virtual void OnGetStateAsJson(String& message);
};

// #############################
// Aborted 
// #############################
class StateMachineState_Aborted: public StateMachineState_Base
{
private:
  unsigned long _stopMillis;
public:
  StateMachineState_Aborted(Settings* pSettings, Horn* pHorn) :StateMachineState_Base(pSettings, pHorn) {DEBUGLOGLN("CTOR: Aborted: ");};
protected:
  virtual void OnStarted();
  virtual EStates OnLoop();
  virtual void OnGetStateAsJson(String& message);
};

#endif
