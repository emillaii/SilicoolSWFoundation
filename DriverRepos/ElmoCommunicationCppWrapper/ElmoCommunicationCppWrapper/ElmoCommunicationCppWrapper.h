#pragma once

#include <iostream>

using namespace System;
using namespace System::Reflection;
using namespace ElmoCommunicationCSharpWrapper;

#define ELMO_API __declspec(dllexport)
// #define LOG_ELMO_CPP
	
ELMO_API void elmo_createInstance(const char* name);
ELMO_API void elmo_deleteInstance(const char* name);
ELMO_API bool elmo_connect(const char* name, const char* ip, const char* hostIp, int port, unsigned char* errMsg);
ELMO_API void elmo_disconnect(const char* name);
ELMO_API bool elmo_sendCommand(const char* name, const char* command, unsigned char* response, unsigned char* errMsg);
