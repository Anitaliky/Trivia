#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "User.h"

class RecievedMessage
{
public:
	RecievedMessage(SOCKET, int);
	RecievedMessage(SOCKET, int, std::vector<std::string>);
	SOCKET getSock();
	int getUserId();
	void setUser(int);
	int getMessageCode();
	std::vector<std::string>& getValues();

private:
	SOCKET _sock;
	int _userId;
	int _messageCode;
	std::vector<std::string> _values;
};