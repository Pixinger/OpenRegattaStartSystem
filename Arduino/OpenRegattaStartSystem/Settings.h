
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Logger.h"

class Settings
{
private:
  int _ClassFlag;
  int _PrepareMinutes;
  int _CountdownMinutes;
public:
  Settings()
    :_ClassFlag(1) // Define the PRESET here!
    ,_PrepareMinutes(2) // Define the PRESET here!
    ,_CountdownMinutes(4) // Define the PRESET here!
    {}  
public:
  void Save() {};
  static Settings* Load(){return new Settings();};
public:
  void setClassFlag(int classFlag) 
  { 
    _ClassFlag = 0;
    if (classFlag < 0) 
      classFlag = 0;
    if (classFlag > 6) 
      classFlag = 6;
    _ClassFlag = classFlag;
  };
  int getClassFlag() 
  { 
    return _ClassFlag;
  };
  void setPrepareMinutes(int prepareMinutes) 
  { 
    if (prepareMinutes < 1)
      prepareMinutes = 1;
    else if (prepareMinutes > 10)
      prepareMinutes = 10;

    _PrepareMinutes = prepareMinutes;
  };
  int getPrepareMinutes() 
  { 
    return _PrepareMinutes;
  };
  void setCountdownMinutes(int countdownMinutes) 
  { 
    if (countdownMinutes < 1)
      countdownMinutes = 1;
    else if (countdownMinutes > 10)
      countdownMinutes = 10;

    _CountdownMinutes = countdownMinutes;
  };
  int getCountdownMinutes() 
  { 
    return _CountdownMinutes;
  };  
};

#endif
