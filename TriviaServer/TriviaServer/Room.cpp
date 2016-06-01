#include "Room.h"

Room::Room(int id, User* admin, std::string name, int maxUsers, int questionNo, int questionTime)
{
	_id = id;
	_admin = admin;
	_name = name;
	_maxUsers = maxUsers;
	_questionNo = questionNo;
	_questionTime = questionTime;
	_users.push_back(admin);
}

bool Room::joinRoom(User* user)
{
	std::string message = std::to_string((int)ServerMessageCode::JOIN_ROOM);
	if (_users.size() < _maxUsers)
	{
		_users.push_back(user);
		message += SUCCESS + Helper::getPaddedNumber(_questionNo, 2) + Helper::getPaddedNumber(_questionTime, 2);
		user->send(message);
		sendMessage(user, getUsersListMessage());
		return true;
	}
	user->send(message + FAIL1);
	return false;
}

void Room::leaveRoom(User* user)
{
	for (int i = 0; i < _users.size(); i++)
		if (user == _users[i])
		{
			_users.erase(_users.begin() + i);
			user->send(std::to_string((int)ServerMessageCode::LEAVE_ROOM) + SUCCESS);
		}
	sendMessage(user, getUsersListMessage());
}

int Room::closeRoom(User* user)
{
	if (user == _admin)
	{
		sendMessage(std::to_string((int)ServerMessageCode::CLOSE_ROOM));
		for (int i = 0; i < _users.size(); i++)
			if (_users[i] != _admin)
				_users[i]->clearRoom();
		return _id;
	}
	return INVALID_ID;
}

std::vector<User*> Room::getUsers()
{
	return _users;
}

std::string Room::getUsersListMessage()
{
	std::string message = std::to_string((int)ServerMessageCode::USERS_ROOM_LIST) + Helper::getPaddedNumber(_users.size(), 1);
	for (int i = 0; i < _users.size(); i++)
		message += Helper::getPaddedNumber(_users[i]->getUsername().length(), 2) + _users[i]->getUsername();
	return message;
}

int Room::getQuestionsNo()
{
	return _questionNo;
}

int Room::getId()
{
	return _id;
}

std::string Room::getName()
{
	return _name;
}

int Room::getQuestionTime()
{
	return _questionTime;
}

void Room::sendMessage(User* excludeUser, std::string message)
{
	for (int i = 0; i < _users.size(); i++)
		if (_users[i] != excludeUser)
		{
			try
			{
				_users[i]->send(message);
			}
			catch (std::exception ex)
			{
				std::cout << ex.what() << std::endl;
			}
		}
}

void Room::sendMessage(std::string message)
{
	sendMessage(nullptr, message);
}