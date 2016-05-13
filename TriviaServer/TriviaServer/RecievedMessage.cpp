#include "RecievedMessage.h"

RecievedMessage::RecievedMessage(SOCKET sock, int messageCode)
{
	_sock = sock;
	_messageCode = messageCode;
	_user = nullptr;
}

RecievedMessage::RecievedMessage(SOCKET sock, int messageCode, std::vector<std::string> values)
{
	_sock = sock;
	_messageCode = messageCode;
	_user = nullptr;
	//TODO
}

SOCKET RecievedMessage::getSock()
{
	return _sock;
}

User* RecievedMessage::getUser()
{
	return _user;
}

void RecievedMessage::setUser(User* user)
{
	_user = user;
}

int RecievedMessage::getMessageCode()
{
	return _messageCode;
}

std::vector<std::string>& RecievedMessage::getValues()
{
	return _values;
}