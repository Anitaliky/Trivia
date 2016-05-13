#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "User.h"
#include "DataBase.h"
#include "Question.h"
#include "DataBase.h"

class Game
{
public:
	Game(const std::vector<User*>& players, int questionsNo, DataBase& db);
	~Game();
	void sendQuestionToAllUsers();
	void handleFinishGame();
	void sendFirstQuestion();
	bool handleNextTurn();
	bool handleAnswerFromUser(User* user, int answerNo, int time);
	bool leaveGame(User* currUser);
	int getId();

private:
	std::vector<Question*> _questions;
	std::vector<User*> _players;
	int _questions_no;
	int _currQuestionIndex;
	DataBase& _db;
	std::map<std::string, int> _results;
	int _currentTurnAnswers;

	bool insertGameToDB();
	void initQuestionsFromDB();
	void sendQuestionToAllUsers();
};