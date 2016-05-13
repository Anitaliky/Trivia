#include "TriviaServer.h"
#include "SafeQueue.h"

TriviaServer::TriviaServer()
{
	//TODO - database constructor
	_roomIdSequence = 0;
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
		throw std::exception("socket failed");
	_socket = s;
}

TriviaServer::~TriviaServer()
{
	if (closesocket(_socket) == SOCKET_ERROR)
		throw std::exception("close socket failed");
}

void TriviaServer::serve(struct addrinfo* a)
{
	bindAndListen(a);
	std::thread(&handleRecievedMessages, this).detach();
	while (true)
	{
		try
		{
			Accept();
		}
		catch (std::exception ex)
		{
			printException(ex);
		}
	}
}

void TriviaServer::bindAndListen(struct addrinfo* a) throw(...)
{
	if (bind(_socket, a->ai_addr, (int)a->ai_addrlen) == SOCKET_ERROR)
		throw std::exception("socket binding failed");
	freeaddrinfo(a);
	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception("socket listening failed");
}

void TriviaServer::Accept()
{
	SOCKADDR_IN sockServ;
	int sockServLen = sizeof(sockServ);
	SOCKET clientSocket = accept(_socket, (SOCKADDR*)&sockServ, &sockServLen);
	if (clientSocket == INVALID_SOCKET)
	{
		if (closesocket(_socket) == SOCKET_ERROR)
			throw std::exception("close socket failed");
		throw std::exception("socket accept failed");
	}
	std::cout << "Client connected!" << std::endl;
	User newUser("", clientSocket);
	//newUser->runThread();
}

//thread's function
void TriviaServer::clientHandler(SOCKET client_socket)
{

}

Room& TriviaServer::getRoomById(int id)
{
	return _roomsList.at(id);
}

User& TriviaServer::getUserByName(std::string name)
{
	for (auto& x : _connectedUsers)
		if (x.second.getUsername() == name)
			return x.second;
}

User& TriviaServer::getUserBySocket(SOCKET client_socket)
{
	return _connectedUsers.at(client_socket);
}

void TriviaServer::safeDeleteUser(RecievedMessage& message)
{

}

//thread's function
void TriviaServer::handleRecievedMessages()
{
	RecievedMessage message = _queRcvMessages.dequeue();
	switch (message.getMessageCode())
	{
	case (int)ClientMessageCode::SIGN_IN:
		handleSignin(message);
		break;
	case (int)ClientMessageCode::SIGN_OUT:
		handleSignout(message);
		break;
	case (int)ClientMessageCode::SIGN_UP:
		handleSignup(message);
		break;
	case (int)ClientMessageCode::LEAVE_GAME:
		handleLeaveGame(message);
		break;
	case (int)ClientMessageCode::START_GAME:
		handleStartGame(message);
		break;
	case (int)ClientMessageCode::ANSWER:
		handlePlayerAnswer(message);
		break;
	case (int)ClientMessageCode::CREATE_ROOM:
		handleCreateRoom(message);
		break;
	case (int)ClientMessageCode::CLOSE_ROOM:
		handleCloseRoom(message);
		break;
	case (int)ClientMessageCode::JOIN_ROOM:
		handleJoinRoom(message);
		break;
	case (int)ClientMessageCode::LEAVE_ROOM:
		handleLeaveRoom(message);
		break;
	case (int)ClientMessageCode::USERS_ROOM_LIST:
		handleGetUsersInRoom(message);
		break;
	case (int)ClientMessageCode::ROOM_LIST:
		handleGetRooms(message);
		break;
	case (int)ClientMessageCode::BEST_SCORES:
		handleGetBestScores(message);
		break;
	case (int)ClientMessageCode::PERSONAL_STATUS:
		handleGetPersonalStatus(message);
		break;
	default:
		break;
	}
}

User& TriviaServer::handleSignin(RecievedMessage& message)				//message No. 200
{

}

void TriviaServer::handleSignout(RecievedMessage& message)				//message No. 201
{

}

bool TriviaServer::handleSignup(RecievedMessage& message)				//message No. 203
{

}

void TriviaServer::handleLeaveGame(RecievedMessage& message)			//message No. 222
{

}

void TriviaServer::handleStartGame(RecievedMessage& message)			//message No. 217
{

}

void TriviaServer::handlePlayerAnswer(RecievedMessage& message)			//message No. 219
{

}

bool TriviaServer::handleCreateRoom(RecievedMessage& message)			//message No. 213
{

}

bool TriviaServer::handleCloseRoom(RecievedMessage& message)			//message No. 215
{

}

bool TriviaServer::handleJoinRoom(RecievedMessage& message)				//message No. 209
{

}

bool TriviaServer::handleLeaveRoom(RecievedMessage& message)			//message No. 211
{

}

void TriviaServer::handleGetUsersInRoom(RecievedMessage& message)		//message No. 207
{

}

void TriviaServer::handleGetRooms(RecievedMessage& message)			//message No. 205
{

}

void TriviaServer::handleGetBestScores(RecievedMessage& message)		//message No. 223
{

}

void TriviaServer::handleGetPersonalStatus(RecievedMessage& message)	//message No. 225
{

}

void TriviaServer::addRecievedMessage(RecievedMessage& message)		//message No. 225
{

}

void TriviaServer::buildRecieveMessage(RecievedMessage& message)		//message No. 225
{

}

//this function is called in 'catch' in the try-catch block
void printException(std::exception ex)
{
	std::cout << ex.what() << std::endl;
	WSACleanup();
	system("pause");
}