#include "Debug.h"

#include "Engine.h"
extern S_Engine Engine;

S_Error::S_Error()
{
	this->Level=0;
}

S_Error::S_Error(std::string nMessage,unsigned int nLevel)
{
	this->Message=nMessage;
	this->Level=nLevel;
}