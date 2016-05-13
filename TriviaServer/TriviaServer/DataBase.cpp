#include "DataBase.h"

DataBase::DataBase()
{
	if (sqlite3_open("trivia.db", &_dataBase))
		throw std::exception();
}

DataBase::~DataBase()
{

}

bool DataBase::isUserExists(std::string username)
{

}

bool DataBase::addNewUser(std::string username, std::string password, std::string email)
{

}

bool DataBase::isUserAndPassMatch(std::string username, std::string password)
{

}

std::vector<Question*> DataBase::initQuestions(int questionsNo)
{

}

std::vector<std::string> DataBase::getBestScores()
{

}

std::vector<std::string> DataBase::getPersonalStatus()
{

}

int DataBase::insertNewGame()
{

}

bool DataBase::updateGameStatus(int)
{

}

bool DataBase::addAnswerToPlayer(int gameId, std::string username, int questionId, std::string answer, bool isCorrect, int answerTime)
{

}



int DataBase::callbackCount(void*, int, char**, char**)
{

}

int DataBase::callbackQuestions(void*, int, char**, char**)
{

}

int DataBase::callbackBestScores(void*, int, char**, char**)
{

}

int DataBase::callbackPersonStatus(void*, int, char**, char**)
{

}