#include "stdafx.h"

#include "ElmoCommunicationCppWrapper.h"

ELMO_API void elmo_createInstance(const char * name)
{
#ifdef LOG_ELMO_CPP
	std::cout << "elmo---" << "elmo_createInstance" << " " << name << std::endl;
#endif // LOG_ELMO_CPP

	ElmoWrapperManager::createInstance(gcnew String(name));
}

ELMO_API void elmo_deleteInstance(const char * name)
{
#ifdef LOG_ELMO_CPP
	std::cout << "elmo---" << "elmo_deleteInstance" << " " << name << std::endl;
#endif // LOG_ELMO_CPP

	ElmoWrapperManager::deleteInstance(gcnew String(name));
}

ELMO_API bool elmo_connect(const char * name, const char * ip, const char * hostIp, int port, unsigned char* errMsg)
{
#ifdef LOG_ELMO_CPP
	std::cout << "elmo---" << "elmo_connect" << " " << name << " " << ip << " " << hostIp << " " << port << std::endl;
#endif // LOG_ELMO_CPP

	return ElmoWrapperManager::connect(gcnew String(name), gcnew String(ip), gcnew String(hostIp), port, errMsg);
}

ELMO_API void elmo_disconnect(const char * name)
{
	ElmoWrapperManager::disconnect(gcnew String(name));
}

ELMO_API bool elmo_sendCommand(const char * name, const char * command, unsigned char * response, unsigned char * errMsg)
{
#ifdef LOG_ELMO_CPP
	std::cout << "elmo---" << "elmo_sendCommand" << " " << name << " " << command << std::endl;
#endif // LOG_ELMO_CPP

	return ElmoWrapperManager::sendCommand(gcnew String(name), gcnew String(command), response, errMsg);
}
