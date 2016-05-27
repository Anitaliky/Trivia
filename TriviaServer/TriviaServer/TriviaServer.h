#pragma once
#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <map>
#include <thread>
#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include "Helper.h"
#include "Validator.h"
#include "Protocol.h"
#include "User.h"
#include "Room.h"
#include "RecievedMessage.h"
#include "Database.h"
#include "Game.h"
#include "SafeQueue.h"

class TriviaServer
{
public:
	TriviaServer() throw(...);
	~TriviaServer() throw(...);
	void serve(struct addrinfo*);
	void bindAndListen(struct addrinfo*) throw(...);
	void Accept() throw(...);
	void clientHandler(SOCKET);
	Room* getRoomById(int);
	User* getUserByName(std::string);
	User* getUserBySocket(SOCKET);
	void safeDeleteUser(RecievedMessage*);
	void handleRecievedMessages();
	bool handleSignin(RecievedMessage*);			//message No. 200
	void handleSignout(RecievedMessage*);			//message No. 201
	bool handleSignup(RecievedMessage*);			//message No. 203
	void handleLeaveGame(RecievedMessage*);			//message No. 222
	void handleStartGame(RecievedMessage*);			//message No. 217
	void handlePlayerAnswer(RecievedMessage*);		//message No. 219
	bool handleCreateRoom(RecievedMessage*);		//message No. 213
	bool handleCloseRoom(RecievedMessage*);			//message No. 215
	bool handleJoinRoom(RecievedMessage*);			//message No. 209
	bool handleLeaveRoom(RecievedMessage*);			//message No. 211
	void handleGetUsersInRoom(RecievedMessage*);	//message No. 207
	void handleGetRooms(RecievedMessage*);			//message No. 205
	void handleGetBestScores(RecievedMessage*);		//message No. 223
	void handleGetPersonalStatus(RecievedMessage*);	//message No. 225
	RecievedMessage* buildRecieveMessage(SOCKET, int);

private:
	SOCKET _socket;
	std::map<SOCKET, User*> _connectedUsers;
	SafeQueue<RecievedMessage*> _queRcvMessages;
	DataBase _db;
	std::map<int, Room*> _roomsList;
	static int _roomIdSequence;

	void getValues(SOCKET, std::vector<std::string>&);
};