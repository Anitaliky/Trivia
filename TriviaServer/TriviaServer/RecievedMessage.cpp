#include "RecievedMessage.h"

RecievedMessage::RecievedMessage(SOCKET sock, int messageCode)
{
	_sock = sock;
	_messageCode = messageCode;
}

RecievedMessage::RecievedMessage(SOCKET sock, int messageCode, std::vector<std::string> values)
{
	_sock = sock;
	_messageCode = messageCode;
	_values = values;
}

SOCKET RecievedMessage::getSock()
{
	return _sock;
}

int RecievedMessage::getUserId()
{
	return _userId;
}

void RecievedMessage::setUser(int user)
{
	_userId = user;
}

int RecievedMessage::getMessageCode()
{
	return _messageCode;
}

std::vector<std::string>& RecievedMessage::getValues()
{
	return _values;
}