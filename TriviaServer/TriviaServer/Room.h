#pragma once
#include "Helper.h"
#include "User.h"
#include <iostream>
#include <vector>

class Room
{
public:
	Room(int, User*, std::string, int, int, int);
	bool joinRoom(User*);
	void leaveRoom(User*);
	int closeRoom(User*);
	std::vector<User*> getUsers();
	std::string getUsersListMessage();
	int getQuestionsNo();
	int getId();
	std::string getName();

private:
	std::vector<User*> _users;
	User* _admin;
	int _maxUsers;
	int _questionTime;
	int _questionNo;
	std::string _name;
	int _id;
	std::string getUsersAsString(std::vector<User*>, User*);
	void sendMessage(User*, std::string);
	void sendMessage(std::string);

};