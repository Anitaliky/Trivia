#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <exception>
#include <algorithm>
#include "Question.h"
#include "sqlite3.h"

typedef enum class callback_type
{
	COUNT,
	QUESTIONS,
	BEST_SCORES,
	PERSONAL_STATUS,
	PLAYERS_ANSWERS
} CallbackType;

class DataBase
{
public:
	DataBase() throw(...);
	~DataBase();
	bool isUserExists(std::string);
	bool addNewUser(std::string, std::string, std::string);
	bool isUserAndPassMatch(std::string, std::string);
	std::vector<Question*> initQuestions(int);
	std::map<std::string, std::string> getBestScores();		//return maximum 3 scores
	std::vector<std::string> getPersonalStatus(std::string);
	int insertNewGame();
	bool updateGameStatus(int);
	bool addAnswerToPlayer(int, std::string, int, std::string, bool, int);

private:
	int _rc;
	sqlite3* _db;
	char* _zErrMsg;
	std::unordered_map<std::string, std::vector<std::string>> _results;
	std::unordered_map<std::string, std::vector<std::string>> _questions;
	std::unordered_map<std::string, std::vector<std::string>> _playersAnswers;
	std::vector<std::string> _personalStatus;
	std::unordered_map<std::string, std::vector<std::string>> _bestScores;

	static int callbackCount(void*, int, char**, char**);
	static int callbackPersonStatus(void*, int, char**, char**);

	void send_check(CallbackType, sqlite3*, std::stringstream&, char*);
	Question* getQuestionFromMap(int);
	int getMaxElementIndex(std::unordered_map<std::string, std::vector<std::string>>&);
};