#include "NetworkManager.h"
#include "arduino_secrets.h"

const char* NetworkManager::ssid = APSSID;
const char* NetworkManager::pass = APPSK;
NetworkManager* NetworkManager::_spInstance = NULL;

NetworkManager::NetworkManager()
  :_pServer(NULL)
  ,_ServerStatus(WL_IDLE_STATUS)
{  
  if (_spInstance != NULL)
  {
    DEBUGLOGLN("Only one instance of NetworkManager is allowed.");
    while (true); // don't continue 
  }
  
  _spInstance = this;
}

void NetworkManager::Setup(StateMachine* pStateMachine)
{
  _pStateMachine = pStateMachine;

  _pServer = new ESP8266WebServer(80);

  gotSoftAPModeStationConnected = WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected& event)
  {
    Serial.print("AP connected");
  });
  gotSoftAPModeStationDisconnected = WiFi.onSoftAPModeStationDisconnected([](const WiFiEventSoftAPModeStationDisconnected& event)
  {
    Serial.println("AP disconnected");
  });


  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    DEBUGLOGLN("WiFi shield not present. STOPPED");
    while (true); // don't continue
  }

  // print the network name (SSID);
  DEBUGLOG("Creating access point named: ");
  DEBUGLOGLN(ssid);

  // You can remove the password parameter if you want the AP to be open. 
  // WiFi.softAP(ssid, password, channel, hidden, max_connection)
  WiFi.persistent(false);
  if (!WiFi.softAP(ssid, pass, 5, false, 4))
  {
    DEBUGLOGLN("Creating access point failed. STOPPED");
    while (true); // don't continue    
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access point IP address: ");
  Serial.println(myIP);
  _pServer->on("/", []() { _spInstance->OnHandleRoot(); });   
  _pServer->on("/Reset", []() { _spInstance->OnHandleReset(); });   
  _pServer->on("/Emergency", []() { _spInstance->OnHandleEmergency(); });   
  _pServer->on("/Start",[]() { _spInstance->OnHandleStart(); });   
  _pServer->on("/AbortSingle", []() { _spInstance->OnHandleAbortSingle(); });   
  _pServer->on("/AbortAll", []() { _spInstance->OnHandleAbortAll(); });   
  _pServer->on("/Abort", []() { _spInstance->OnHandleAbort(); });   
  
  _pServer->begin();
  Serial.println("HTTP server started");
}

void NetworkManager::Loop()
{
  _pServer->handleClient();
}


void NetworkManager::Respond_Code(int code, const char* reason, const char* response_message)
{
    String message = "{\n";
    if (response_message != NULL)
    {
      message += "\"msg\" : \"";
      message += response_message;
      message += "\"";
    }
    message += "}\n";
    _pServer->send(code, "application/json", message);
}
void NetworkManager::Respond_OK()
{
  Respond_Code(200, "OK");
  DEBUGLOGLN("- Respond_OK");
}
void NetworkManager::Respond_BadRequest(const char* response_message)
{
  Respond_Code(400, "Bad Request", response_message);
  DEBUGLOGLN("- Respond_BadRequest");
}

bool NetworkManager::ProcessRequest_Start(String & request)
{/*
  // Check if length is OK
  if (request.length() < 11)
  {
    DEBUGLOGLN("Error: length < 11");
    return false;      
  }
  
  // Check if '/' is at 10th position
  char c = request.charAt(10);
  if (c == '/')
  {
  }
  else
  {
    WARNLOGLN("Error: Invalid char at position 10");
    return false;      
  }
  
  // ClassFlagId
  int index1 = 11;      
  int index2 = request.indexOf("/", index1);
  if (index2 == -1)
  {
    WARNLOGLN("Error: ClassFlagId '/' index");
    return false;      
  }
  String sClassFlagId = request.substring(index1, index2);
  int iClassFlagId = sClassFlagId.toInt();      
  if ((iClassFlagId < 0) || (iClassFlagId > 0))
  {
    WARNLOGLN("Error: iClassFlagId out of range");
    return false;      
  }
  
  // PrepareMinutes
  index1 = index2 + 1;
  index2 = request.indexOf("/", index1);
  if (index2 == -1)
  {
    WARNLOGLN("Error: PrepareMinutes '/' index");
    return false;      
  }
  String sPrepareMinutes = request.substring(index1, index2);
  int iPrepareMinutes = sPrepareMinutes.toInt();      
  if ((iPrepareMinutes < 1) || (iPrepareMinutes > 10))
  {
    WARNLOGLN("Error: iPrepareMinutes out of range");
    return false;      
  }
  
  // CountdownMinutes
  index1 = index2 + 1;
  index2 = request.indexOf(" ", index1);
  if (index2 == -1)
  {
    WARNLOGLN("Error: CountdownMinutes '/' index");
    return false;      
  }
  String sCountdownMinutes = request.substring(index1, index2);
  int iCountdownMinutes = sCountdownMinutes.toInt();      
  if ((iCountdownMinutes < 1) || (iCountdownMinutes > 10))
  {
    WARNLOGLN("Error: iCountdownMinutes out of range");
    return false;      
  }
  
  DEBUGLOG("iClassFlagId: ");DEBUGLOG(iClassFlagId); DEBUGLOG(" iPrepareMinutes: ");DEBUGLOG(iPrepareMinutes); DEBUGLOG(" iCountdownMinutes: ");DEBUGLOGLN(iCountdownMinutes);
  return _pStateMachine->Start(iClassFlagId, iPrepareMinutes, iCountdownMinutes);*/
  return true;
}


void NetworkManager::OnHandleRoot()
{  
  DEBUGLOGLN("OnHandleRoot");
  String message = "{\n";
  _pStateMachine->GetCurrentStateAsJson(message); 
  message += "}\n";
  _pServer->send(200, "application/json", message);
}
void NetworkManager::OnHandleReset()
{  
  DEBUGLOGLN("OnHandleReset");
  if (!_pStateMachine->Reset())
    Respond_BadRequest();
  else
    Respond_OK();
}
void NetworkManager::OnHandleEmergency()
{    
  DEBUGLOGLN("OnHandleEmergency");
  if (!_pStateMachine->Emergency())
    Respond_BadRequest();
  else
    Respond_OK();  
}
void NetworkManager::OnHandleStart()
{  
  DEBUGLOGLN("OnHandleStart");

  // ClassFlag
  String cf = _pServer->argName(0);
  if (!cf.equals("cf"))
  {
    DEBUGLOGLN("CF is empty");
    Respond_BadRequest();
    return;
  }
  cf = _pServer->arg(0);
  if (cf.isEmpty())
  {
    DEBUGLOGLN("CF is empty");
    Respond_BadRequest();
    return;
  }
  int iClassFlagId = cf.toInt();      
  if ((iClassFlagId < 0) || (iClassFlagId > 0))
  {
    WARNLOGLN("Error: iClassFlagId out of range");
    Respond_BadRequest();
    return;      
  }

  // PrepareMinutes
  String pm = _pServer->argName(1);
  if (!pm.equals("pm"))
  {
    DEBUGLOGLN("PM is empty");
    Respond_BadRequest();
    return;
  }
  pm = _pServer->arg(1);
  if (pm.isEmpty())
  {
    DEBUGLOGLN("PM is empty");
    Respond_BadRequest();
    return;
  }
  int iPrepareMinutes = pm.toInt();      
  if ((iPrepareMinutes < 1) || (iPrepareMinutes > 10))
  {
    WARNLOGLN("Error: iPrepareMinutes out of range");
    Respond_BadRequest();
    return;      
  }

  // CountdownMinutes
  String cm = _pServer->argName(2);
  if (!cm.equals("cm"))
  {
    DEBUGLOGLN("CM is empty");
    Respond_BadRequest();
    return;
  }
  cm = _pServer->arg(2);
  if (cm.isEmpty())
  {
    DEBUGLOGLN("CM is empty");
    Respond_BadRequest();
    return;
  }
  int iCountdownMinutes = cm.toInt();      
  if ((iCountdownMinutes < 1) || (iCountdownMinutes > 10))
  {
    WARNLOGLN("Error: iCountdownMinutes out of range");
    Respond_BadRequest();
    return;      
  }
  
  DEBUGLOG("iClassFlagId: ");DEBUGLOG(iClassFlagId); DEBUGLOG(" iPrepareMinutes: ");DEBUGLOG(iPrepareMinutes); DEBUGLOG(" iCountdownMinutes: ");DEBUGLOGLN(iCountdownMinutes);
  if (!_pStateMachine->Start(iClassFlagId, iPrepareMinutes, iCountdownMinutes))
  {
    DEBUGLOGLN("Start() returned false");
    Respond_BadRequest();
    return;      
  }

  Respond_OK();
}
void NetworkManager::OnHandleAbortSingle()
{  
  DEBUGLOGLN("OnHandleAbortSingle");
  if (!_pStateMachine->AbortSingle())
    Respond_BadRequest();
  else
    Respond_OK();
}
void NetworkManager::OnHandleAbortAll()
{  
  DEBUGLOGLN("OnHandleAbortAll");
  if (!_pStateMachine->AbortAll())
    Respond_BadRequest();
  else
    Respond_OK();
}
void NetworkManager::OnHandleAbort()
{  
  DEBUGLOGLN("OnHandleAbort");
  if (!_pStateMachine->Abort())
    Respond_BadRequest();
  else
    Respond_OK();
}

void NetworkManager::OnHandleNotFound() 
{
  DEBUGLOGLN("OnHandleNotFound");
  Respond_BadRequest();
}
