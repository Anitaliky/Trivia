#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "Question.h"

class DataBase
{
public:
	DataBase() throw(...);
	~DataBase();
	bool isUserExists(std::string username);
	bool addNewUser(std::string username, std::string password, std::string email);
	bool isUserAndPassMatch(std::string username, std::string password);
	std::vector<Question*> initQuestions(int questionsNo);
	std::vector<std::string> getBestScores();
	std::vector<std::string> getPersonalStatus();
	int insertNewGame();
	bool updateGameStatus(int);
	bool addAnswerToPlayer(int gameId, std::string username, int questionId, std::string answer, bool isCorrect, int answerTime);

private:
	static int callbackCount(void*, int, char**, char**);
	static int callbackQuestions(void*, int, char**, char**);
	static int callbackBestScores(void*, int, char**, char**);
	static int callbackPersonStatus(void*, int, char**, char**);
	sqlite3* _dataBase;
};