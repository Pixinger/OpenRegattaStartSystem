#include "Horn.h"

#define MAX_HORN_SIGNAL_COUNT 3
#define MAX_HORN_STEP_COUNT  MAX_HORN_SIGNAL_COUNT*2
#define SHORT_HORN_DURATION_ON_MS  500
#define SHORT_HORN_DURATION_OFF_MS  500
#define LONG_HORN_DURATION_ON_MS   1500
#define LONG_HORN_DURATION_OFF_MS   1500

#define HORN_LED_PIN  LED_BUILTIN
#define HORN_RELAY_PIN  5


Horn::Horn()
{
  //DEBUGLOGf("Horn-Pointer = %p\n", this);
  _paSteps = new Horn::Step*[MAX_HORN_STEP_COUNT];
  for (int i = 0; i < MAX_HORN_STEP_COUNT; i++)
    _paSteps[i] = NULL;
}
Horn::~Horn()
{
  Clear(); // Release all existsing steps
  delete [] _paSteps; // Release array
  _paSteps = NULL;
}

void Horn::Setup()
{
  pinMode(HORN_LED_PIN, OUTPUT);       // set the LED pin mode
  pinMode(HORN_RELAY_PIN, OUTPUT);       // set the LED pin mode
  SetIOs(false);
}

void Horn::Loop()
{
  if (_paSteps[0] != NULL)
  {
    if (_paSteps[0]->ActionMillis <= millis())
    {
      SetIOs(_paSteps[0]->Active);

      // Move all entries down in the queue
      delete _paSteps[0];
      for (int i = 0; i < MAX_HORN_STEP_COUNT-1; i++)
      {
        _paSteps[i] = _paSteps[i+1];
      }
      _paSteps[MAX_HORN_STEP_COUNT-1] = NULL;   
            
    }  
  }
}

void Horn::SetIOs(bool on)
{
  if (on)
  {
    DEBUGLOGLN("Horn::SetIOs(ON)");
    digitalWrite(HORN_LED_PIN, LOW);
    digitalWrite(HORN_RELAY_PIN, HIGH);
  }
  else
  {
    DEBUGLOGLN("Horn::SetIOs(OFF)");
    digitalWrite(HORN_LED_PIN, HIGH);
    digitalWrite(HORN_RELAY_PIN, LOW);
  }
}

void Horn::Clear()
{
  for (int i = 0; i < MAX_HORN_STEP_COUNT; i++)
  {
    if (_paSteps[i] != NULL)
    {
      delete _paSteps[i];
      _paSteps[i] = NULL;
    }
  }
  
  SetIOs(false);  
}
void Horn::Off()
{
  DEBUGLOGLN("Horn Off");
  Clear();
}
void Horn::Short(int count)
{
  if (count < 1)
    count = 1;
  else if (count > MAX_HORN_SIGNAL_COUNT)
    count = MAX_HORN_SIGNAL_COUNT;
    
  DEBUGLOG("Horn:Short("); DEBUGLOG(count); DEBUGLOGLN(")");

  Clear();

  int iStep = 0;
  unsigned long now = millis();
  for (int i = 0; i < count; i++)
  {
    _paSteps[iStep++] = new Step(true,  now);
    now += SHORT_HORN_DURATION_ON_MS;
    _paSteps[iStep++] = new Step(false, now);
    now += SHORT_HORN_DURATION_OFF_MS;
  }

}
void Horn::Long(int count)
{  
  if (count < 1)
    count = 1;
  else if (count > MAX_HORN_SIGNAL_COUNT)
    count = MAX_HORN_SIGNAL_COUNT;
  
  DEBUGLOG("Horn:Long("); DEBUGLOG(count); DEBUGLOGLN(")");

  Clear();

  int iStep = 0;
  unsigned long now = millis();
  for (int i = 0; i < count; i++)
  {
    _paSteps[iStep++] = new Step(true,  now);
    now += LONG_HORN_DURATION_ON_MS;
    _paSteps[iStep++] = new Step(false, now);
    now += LONG_HORN_DURATION_OFF_MS;
  }
}
