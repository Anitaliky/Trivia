#include "TriviaServer.h"
#include "SafeQueue.h"

TriviaServer::TriviaServer() : _db()
{
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
	std::thread(&TriviaServer::handleRecievedMessages, this).detach();
	while (true)
	{
		try
		{
			Accept();
		}
		catch (std::exception ex)
		{
			std::cout << ex.what() << std::endl;
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
	std::cout << "Client connected with socket" << clientSocket << std::endl;
	User* newUser = new User("", clientSocket);
	_connectedUsers.insert(std::pair<SOCKET, User*>(clientSocket, newUser));
	std::thread(&TriviaServer::clientHandler, this, std::ref(clientSocket)).detach();
}

//thread's function
void TriviaServer::clientHandler(SOCKET client_socket)
{
	int msgCode = Helper::getMessageTypeCode(client_socket);
	while (msgCode || msgCode != (int)ClientMessageCode::EXIT)
	{
		
		RecievedMessage* newMessage = buildRecieveMessage(client_socket, msgCode);
		_queRcvMessages.enqueue(newMessage);
	}
}

Room* TriviaServer::getRoomById(int id)
{
	return _roomsList.at(id);
}

User* TriviaServer::getUserByName(std::string name)
{
	for (auto& x : _connectedUsers)
		if (x.second->getUsername() == name)
			return x.second;
	return nullptr;
}

User* TriviaServer::getUserBySocket(SOCKET client_socket)
{
	return _connectedUsers.at(client_socket);
}

void TriviaServer::safeDeleteUser(RecievedMessage* message)
{

}

//thread's function
void TriviaServer::handleRecievedMessages()
{
	RecievedMessage* message = _queRcvMessages.dequeue();
	std::cout << "message code is:" << message->getMessageCode() << std::endl;
	switch (message->getMessageCode())
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

bool TriviaServer::handleSignin(RecievedMessage* message)
{
	std::string msg = std::to_string((int)ServerMessageCode::SIGN_IN);
	std::vector<std::string> user_pass = message->getValues();
	if (_db.isUserAndPassMatch(user_pass[0], user_pass[1]))
	{
		if (_connectedUsers.find(message->getSock()) == _connectedUsers.end())
		{
			User* newUser = new User(user_pass[0], message->getSock());
			std::pair<SOCKET, User*> user;
			user.first = message->getSock();
			user.second = newUser;
			_connectedUsers.insert(user);
			newUser->send(msg + SUCCESS);
			return true;
		}
		_connectedUsers.find(message->getSock())->second->send(msg + FAIL2);
	}
	getUserByName(user_pass[0])->send(msg + FAIL1);
	return false;
}

void TriviaServer::handleSignout(RecievedMessage* message)
{
	std::map<SOCKET, User*>::iterator it = _connectedUsers.find(message->getSock());
	if (it != _connectedUsers.end())
	{
		_connectedUsers.erase(it);
		if (handleCloseRoom(message))
			if (handleLeaveRoom(message))
				handleLeaveGame(message);
	}
}

bool TriviaServer::handleSignup(RecievedMessage* message)
{
	std::string msg = std::to_string((int)ServerMessageCode::SIGN_UP);
	if (Validator::isPasswordValid(message->getValues()[1]))
	{
		if (Validator::isUsernameValid(message->getValues()[0]))
		{
			if (!_db.isUserExists(message->getValues()[0]))
			{
				if (_db.addNewUser(message->getValues()[0], message->getValues()[1], message->getValues()[2]))
				{
					getUserByName(message->getValues()[0])->send(msg + SUCCESS);
					return true;
				}
				getUserByName(message->getValues()[0])->send(msg + FAIL4);
				return false;
			}
			getUserByName(message->getValues()[0])->send(msg + FAIL2);
			return false;
		}
		getUserByName(message->getValues()[0])->send(msg + FAIL3);
		return false;
	}
	getUserByName(message->getValues()[0])->send(msg + FAIL1);
	return false;
}

void TriviaServer::handleLeaveGame(RecievedMessage* message)
{
	getUserBySocket(message->getSock())->leaveGame();
}

void TriviaServer::handleStartGame(RecievedMessage* message)
{
	User* user = getUserBySocket(message->getSock());
	std::vector<User*> players;
	for (std::map<SOCKET, User*>::iterator it = _connectedUsers.begin(); it != _connectedUsers.end(); ++it)
		players.push_back(it->second);

	try
	{
		Game* game = new Game(players, user->getRoom()->getQuestionsNo(), _db, user->getRoom()->getId());
		_roomsList.erase(user->getRoom()->getId());
		game->sendFirstQuestion();
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		user->send(/*fail message*/"");
	}

}

void TriviaServer::handlePlayerAnswer(RecievedMessage* message)
{
	std::vector<std::string> answer_time = message->getValues();
	Game* game = getUserBySocket(message->getSock())->getGame();
	if (game)
		game->handleAnswerFromUser(getUserBySocket(message->getSock()), std::stoi(answer_time[0]), std::stoi(answer_time[1]));
}

bool TriviaServer::handleCreateRoom(RecievedMessage* message)
{
	User* user = getUserBySocket(message->getSock());
	if (user)
	{
		std::vector<std::string> values = message->getValues();
		if (user->createRoom(++_roomIdSequence, values[0], std::stoi(values[1]), std::stoi(values[2]), std::stoi(values[3])))
		{
			std::pair<int, Room*> newRoom;
			newRoom.first = _roomIdSequence;
			newRoom.second = user->getRoom();
			_roomsList.insert(newRoom);
			return true;
		}
	}
	return false;
}

bool TriviaServer::handleCloseRoom(RecievedMessage* message)
{
	Room* room = getUserBySocket(message->getSock())->getRoom();
	if (room)
	{
		int roomID = getUserBySocket(message->getSock())->closeRoom();
		if (roomID != -1)
		{
			_roomsList.erase(roomID);
			return true;
		}
	}
	return false;
}

bool TriviaServer::handleJoinRoom(RecievedMessage* message)
{
	std::string msg = std::to_string((int)ServerMessageCode::JOIN_ROOM);
	User* user = getUserBySocket(message->getSock());
	if (user)
	{
		std::string roomID = message->getValues()[0];
		Room* room = getRoomById(std::stoi(roomID));
		if (room)
		{
			if (user->joinRoom(room))
			{
				user->send(msg + SUCCESS + std::to_string(room->getQuestionsNo()) + std::to_string(room->getQuestionTime()));
				return true;
			}
			user->send(msg + FAIL1);
		}
		user->send(msg + FAIL2);
	}
	return false;
}

bool TriviaServer::handleLeaveRoom(RecievedMessage* message)
{
	User* user = getUserBySocket(message->getSock());
	if (user)
	{
		Room* room = user->getRoom();
		if (room)
		{
			user->leaveRoom();
			return true;
		}
	}
	return false;
}

void TriviaServer::handleGetUsersInRoom(RecievedMessage* message)
{
	User* user = getUserBySocket(message->getSock());
	if (user)
	{
		std::string roomID = message->getValues()[0];
		if (std::stoi(roomID) != -1)
		{
			Room* room = getRoomById(std::stoi(roomID));
			if (room)
				user->send(room->getUsersListMessage());
		}
		else
			user->send(/*fail message*/"");
	}
}

void TriviaServer::handleGetRooms(RecievedMessage* message)
{
	std::string msg = std::to_string((int)ServerMessageCode::ROOM_LIST) + std::to_string(_roomsList.size());
	for (std::map<int, Room*>::iterator it = _roomsList.begin(); it != _roomsList.end(); ++it)
		msg += std::to_string(it->first) + it->second->getName();
	getUserBySocket(message->getSock())->send(msg);
}

void TriviaServer::handleGetBestScores(RecievedMessage* message)
{
	std::string msg = std::to_string((int)ServerMessageCode::BEST_SCORES);
	std::map<std::string, std::string> bestScores = _db.getBestScores();
	for (std::map<std::string, std::string>::iterator it = bestScores.begin(); it != bestScores.end(); ++it)
		msg += std::to_string(it->first.size()) + it->first + it->second;
	for (int i = bestScores.size(); i < 3; i++)
		msg += "00";
	getUserBySocket(message->getSock())->send(msg);
}

void TriviaServer::handleGetPersonalStatus(RecievedMessage* message)
{
	std::string msg = std::to_string((int)ServerMessageCode::PERSONAL_STATUS);
	std::vector<std::string> status = _db.getPersonalStatus(getUserBySocket(message->getSock())->getUsername());
	for (int i = 0; i < status.size(); i++)
		msg += status[i];
	getUserBySocket(message->getSock())->send(msg);
}

RecievedMessage* TriviaServer::buildRecieveMessage(SOCKET clientSocket, int msgCode)
{
	std::vector<std::string> values;
	switch (msgCode)
	{
	case (int)ClientMessageCode::SIGN_IN:
		for (int i = 0; i < 2; i++)
			getValues(clientSocket, values);
		break;
	case (int)ClientMessageCode::SIGN_UP:
		for (int i = 0; i < 3; i++)
			getValues(clientSocket, values);
		break;
	case (int)ClientMessageCode::ANSWER:
		values.push_back(Helper::getStringPartFromSocket(clientSocket, 1)); //answerNumber 
		values.push_back(Helper::getStringPartFromSocket(clientSocket, 2)); //TimeInSeconds
		break;
	case (int)ClientMessageCode::CREATE_ROOM:
		getValues(clientSocket, values); //roomName 
		values.push_back(Helper::getStringPartFromSocket(clientSocket, 1)); //playersNumber
		for (int i = 0; i < 2; i++) //questionsNumber, questionTimeInSec
			values.push_back(Helper::getStringPartFromSocket(clientSocket, 2));
		break;
	case (int)ClientMessageCode::USERS_ROOM_LIST:
	case (int)ClientMessageCode::JOIN_ROOM:
		values.push_back(Helper::getStringPartFromSocket(clientSocket, 4));
		break;
	default:
		break;
	}
	RecievedMessage* ret = new RecievedMessage(clientSocket, msgCode, values);
	return ret;
}

//a private function for buildRecieveMessage
void TriviaServer::getValues(SOCKET clientSocket, std::vector<std::string> &values)
{
	values.push_back(Helper::getStringPartFromSocket(clientSocket, Helper::getIntPartFromSocket(clientSocket, 2)));
}