#pragma once
#include "Helper.h"
#include <iostream>
#include "Room.h"
#include "Game.h"
#include "Protocol.h"

class Room;
class Game;

class User
{
public:
	User(std::string, SOCKET);
	void send(std::string);
	std::string getUsername();
	SOCKET getSocket();
	Room* getRoom();
	Game* getGame();
	void setGame(Game*);
	void setUsername(std::string);
	void clearRoom();
	bool createRoom(int, std::string, int, int, int);
	bool joinRoom(Room*);
	void leaveRoom();
	int closeRoom();
	bool leaveGame();

private:
	std::string _username;
	Room* _currRoom;
	Game* _currGame;
	SOCKET _sock;
};