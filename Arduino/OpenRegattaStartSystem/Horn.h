#ifndef HORN_H
#define HORN_H

#include <Arduino.h>
#include "Logger.h"

class Horn
{
private:
  class Step
  {
    public:
      bool Active;
      unsigned long ActionMillis; 
    public:
      Step(bool active, unsigned long actionMillis):Active(active), ActionMillis(actionMillis) { };
  };  
private:
  Step** _paSteps;
public:
  Horn();
  ~Horn();
public:
  void Setup();
  void Loop();
public:
  void Off();
  void Short(int count);
  void Long(int count); 
private:
  void Clear();
  void SetIOs(bool on);
};

#endif
