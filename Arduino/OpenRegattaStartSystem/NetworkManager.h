
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Logger.h"
#include "StateMachine.h"

class NetworkManager
{
private:
  static NetworkManager* _spInstance;
private:
  StateMachine* _pStateMachine;
private:  
  static const char* ssid;            // your network SSID (name)
  static const char* pass;            // your network password (use for WPA, or use as key for WEP)
  int _ServerStatus;
  ESP8266WebServer* _pServer;
  WiFiEventHandler  gotSoftAPModeStationConnected;
  WiFiEventHandler   gotSoftAPModeStationDisconnected;
public:
  NetworkManager();
public:
  void Setup(StateMachine* pStateMachine);
  void Loop();
private:
  void Respond_Code(int code, const char* reason, const char* response_message = NULL);
  void Respond_OK();
  void Respond_BadRequest(const char* response_message = NULL);
private:
  void OnHandleNotFound();
  void OnHandleRoot(); // Equal to RequestState
  void OnHandleReset();
  void OnHandleEmergency();
  void OnHandleStart();
  void OnHandleAbortSingle();
  void OnHandleAbortAll();
  void OnHandleAbort();
};

#endif
