#include "User.h"

User::User(std::string username, SOCKET sock)
{
	_username = username;
	_sock = sock;
	_currRoom = nullptr;
	_currGame = nullptr;
}

void User::send(std::string message)
{
	Helper::sendData(_sock, message);
	std::cout << "Sent message: " << message << std::endl;
}

std::string User::getUsername()
{
	return _username;
}

SOCKET User::getSocket()
{
	return _sock;
}

Room* User::getRoom()
{
	return _currRoom;
}

Game* User::getGame()
{
	return _currGame;
}

void User::setGame(Game* currGame)
{
	_currGame = currGame;
}

void User::setUsername(std::string username)
{
	_username = username;
}

void User::clearRoom()
{
	_currGame = nullptr;
}

bool User::createRoom(int roomId, std::string roomName, int maxUsers, int questionsNo, int questionTime)
{
	if (_currRoom)
	{
		send(std::to_string((int)ServerMessageCode::CREATE_ROOM) + FAIL1);
		return false;
	}
	Room* room = new Room(roomId, this, roomName, maxUsers, questionsNo, questionTime);
	_currRoom = room;
	send(std::to_string((int)ServerMessageCode::CREATE_ROOM) + SUCCESS);
	return true;
}

bool User::joinRoom(Room* room)
{
	if (!_currRoom && room->joinRoom(this))
	{
		_currRoom = room;
		return true;
	}
	send(std::to_string((int)ServerMessageCode::CREATE_ROOM) + FAIL1);
	return false;
}

void User::leaveRoom()
{
	_currRoom->leaveRoom(this);
	_currRoom = nullptr;
}

int User::closeRoom()
{
	int id;
	if (_currRoom)
		id = _currRoom->closeRoom(this);
	if (id != INVALID_ID)
	{
		delete _currRoom;
		_currRoom = nullptr;
	}
	return id;
}

bool User::leaveGame()
{
	bool exit;
	if (_currGame)
	{
		exit = _currGame->leaveGame(this);
		_currGame = nullptr;
	}
	return exit;
}

void User::setCurrRoom(Room* room)
{
	_currRoom = room;
}