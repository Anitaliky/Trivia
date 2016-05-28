#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <exception>
#include "User.h"
#include "DataBase.h"
#include "Question.h"
#include "Helper.h"
#include "Protocol.h"

class User;

class Game
{
public:
	Game(const std::vector<User*>&, int, DataBase&) throw(...);
	~Game();
	void handleFinishGame();
	void sendFirstQuestion();
	bool handleNextTurn();
	bool handleAnswerFromUser(User*, int, int);
	bool leaveGame(User*);
	int getID();

private:
	std::vector<Question*> _questions;
	std::vector<User*> _players;
	int _questions_no;
	int _currQuestionIndex;
	DataBase& _db;
	std::map<std::string, int> _results;
	int _currentTurnAnswers;
	int _id;

	bool insertGameToDB();
	void initQuestionsFromDB();
	void sendQuestionToAllUsers();
};