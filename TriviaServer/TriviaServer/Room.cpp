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
	if (_users.size() < _maxUsers)
	{
		_users.push_back(user);
		//TODO
	}
}

void Room::leaveRoom(User* user)
{

}

int Room::closeRoom(User* user)
{

}

std::vector<User*> Room::getUsers()
{
	return _users;
}

std::string Room::getUsersListMessage()
{

}

int Room::getQuestionsNo()
{

}

int Room::getId()
{

}

std::string Room::getName()
{

}

std::string Room::getUsersAsString(std::vector<User*> usersList, User* ExcludeUser)
{

}

void Room::sendMessage(User* excludeUser, std::string message)
{

}

void Room::sendMessage(std::string message)
{

}