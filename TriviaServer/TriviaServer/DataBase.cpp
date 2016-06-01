#pragma once
#include "DataBase.h"

DataBase::DataBase()
{
	_zErrMsg = 0;
	_rc = sqlite3_open("DataBase.db", &_db);
	if (_rc)
	{
		throw("Can't open database: " + std::string(sqlite3_errmsg(_db), 0, 10));
		sqlite3_close(_db);
	}
}

DataBase::~DataBase()
{
	sqlite3_close(_db);
}

bool DataBase::isUserExists(std::string username)
{
	std::stringstream s;
	s << "select username from t_users;";
	send_check(CallbackType::COUNT, _db, s, _zErrMsg);
	auto it = _results.find("username");
	return std::find(it->second.begin(), it->second.end(), username) != it->second.end();
}

bool DataBase::addNewUser(std::string username, std::string password, std::string email)
{
	std::stringstream s;
	auto it = _results.find("username");
	int usersAmmount = it == _results.end() ? 0 : it->second.size();
	s << "insert into t_users values(" << username << "," << password << "," << email << ");";
	send_check(CallbackType::COUNT, _db, s, _zErrMsg);

	s << "select username from t_users where username=" << username << ";";
	send_check(CallbackType::COUNT, _db, s, _zErrMsg);
	it = _results.find("username");
	return it != _results.end() ? it->second.size() == usersAmmount + 1 : false;
}

bool DataBase::isUserAndPassMatch(std::string username, std::string password)
{
	std::stringstream s;
	s << "select password from t_users where username='" << username << "';";
	send_check(CallbackType::COUNT, _db, s, _zErrMsg);
	auto it = _results.find("password");
	return it->second[it->second.size() - 1] == password;
}

std::vector<Question*> DataBase::initQuestions(int questionsNo)
{
	std::stringstream s;
	std::vector<Question*> questions;
	for (int i = 0; i < questionsNo; i++)
	{
		s << "select * from t_questions order by random() limit 1;";
		send_check(CallbackType::QUESTIONS, _db, s, _zErrMsg);
		Question* question = getQuestionFromMap(_questions.find("question_id")->second.size() - 1);
		questions.push_back(question);
	}
	return questions;
}

std::map<std::string, std::string> DataBase::getBestScores()
{
	std::map<std::string, std::string> returnedMap;
	std::stringstream s;
	s << "select username,sum(is_correct) from t_players_answers group by username;";
	send_check(CallbackType::BEST_SCORES, _db, s, _zErrMsg);
	//Gets 3 best scores
	for (int i = 0; i < 3; i++)
	{
		int max_index = getMaxElementIndex(_bestScores);
		returnedMap.insert(std::pair<std::string, std::string>(_bestScores.find("username")->second[max_index], _bestScores.find("sum(is_correct)")->second[max_index]));		//inserts the highest score to the returned map
		//deletes the highest score from the original map in order to find the next highest score in the map
		_bestScores.find("username")->second.erase(_bestScores.find("username")->second.begin() + max_index);
		_bestScores.find("sum(is_correct)")->second.erase(_bestScores.find("sum(is_correct)")->second.begin() + max_index);
	}
	return returnedMap;
}

std::vector<std::string> DataBase::getPersonalStatus(std::string username)
{
	std::vector<std::string> ret;
	//get number of games
	std::stringstream s;
	s << "select count(*) from(select * from t_players_answers where username='" << username << "' group by game_id);";
	send_check(CallbackType::PERSONAL_STATUS, _db, s, _zErrMsg);

	//get number of right answers
	s << "select count(*) from t_players_answers where username='" << username << "' and is_correct=1;";
	send_check(CallbackType::PERSONAL_STATUS, _db, s, _zErrMsg);

	//get number of wrong answers
	s << "select count(*) from t_players_answers where username='" << username << "' and is_correct=0;";
	send_check(CallbackType::PERSONAL_STATUS, _db, s, _zErrMsg);

	//get average time for answers
	s << "select avg(answer_time) from t_players_answers where username='" << username << "';";
	send_check(CallbackType::PERSONAL_STATUS, _db, s, _zErrMsg);

	return _personalStatus;
}

int DataBase::insertNewGame()
{
	std::stringstream s;
	s << "insert into t_games(status,start_time,end_time) values(0,'NOW',NULL);"; //DOESN'T INSERT
	send_check(CallbackType::COUNT, _db, s, _zErrMsg);

	s << "select game_id from t_games;";
	send_check(CallbackType::COUNT, _db, s, _zErrMsg);
	auto it = _results.find("game_id");
	return std::stoi(it->second[it->second.size() - 1]);
}

bool DataBase::updateGameStatus(int gameId)
{
	std::stringstream s;
	s << "update t_game set status=1,end_time='NOW' where game_id=" << gameId << ";";
	send_check(CallbackType::COUNT, _db, s, _zErrMsg);
	return true;
}

bool DataBase::addAnswerToPlayer(int gameId, std::string username, int questionId, std::string answer, bool isCorrect, int answerTime)
{
	std::stringstream s;
	s << "select count(*) from t_players_answers;";
	send_check(CallbackType::PLAYERS_ANSWERS, _db, s, _zErrMsg);
	int num = std::stoi(_playersAnswers.at("count(*)")[_playersAnswers.at("count(*)").size() - 1]);

	s << "insert into t_players_answers values(" << gameId << ",'" << username << "'," << questionId << ",'" << answer << "'," << isCorrect << "," << answerTime << ");";
	send_check(CallbackType::PLAYERS_ANSWERS, _db, s, _zErrMsg);
	
	s << "select count(*) from t_players_answers;";
	send_check(CallbackType::PLAYERS_ANSWERS, _db, s, _zErrMsg);
	return num + 1 == std::stoi(_playersAnswers.at("count(*)")[_playersAnswers.at("count(*)").size() - 1]);
}

std::string DataBase::getScoreByUsername(std::string username)
{
	std::stringstream s;
	s << "select username,sum(is_correct) from t_players_answers where username='" << username << "' group by username;";;
	send_check(CallbackType::BEST_SCORES, _db, s, _zErrMsg);
	auto it = _bestScores.find("username");
	int i = 0;
	while (i < it->second.size() && it->second[i] != username){ i++; }
	return _bestScores.find("sum(is_correct)")->second[i];
}

//private functions
int DataBase::callbackCount(void* resultSet, int argc, char** argv, char** azCol)
{
	std::unordered_map<std::string, std::vector<std::string>>& results = *static_cast<std::unordered_map<std::string, std::vector<std::string>>*>(resultSet);
	for (int i = 0; i < argc; i++)
	{
		auto it = results.find(azCol[i]);
		if (it != results.end())
			it->second.push_back(argv[i]);
		else
		{
			std::pair<std::string, std::vector<std::string>> p;
			p.first = azCol[i];
			p.second.push_back(argv[i]);
			results.insert(p);
		}
	}
	return 0;
}

int DataBase::callbackPersonStatus(void* resultSet, int argc, char** argv, char** azCol)
{
	std::vector<std::string>& personalStatus = *static_cast<std::vector<std::string>*>(resultSet);
	personalStatus.push_back(argv[0]);
	return 0;
}

//Sends a DML function by sqlite3_exec and checks if arrived
void DataBase::send_check(CallbackType callback, sqlite3* db, std::stringstream &s, char *zErrMsg)
{
	int rc;
	switch (callback)
	{
	case CallbackType::COUNT:
		rc = sqlite3_exec(db, s.str().c_str(), callbackCount, &_results, &zErrMsg);
		break;
	case CallbackType::QUESTIONS:
		rc = sqlite3_exec(db, s.str().c_str(), callbackCount, &_questions, &zErrMsg);
		break;
	case CallbackType::BEST_SCORES:
		rc = sqlite3_exec(db, s.str().c_str(), callbackCount, &_bestScores, &zErrMsg);
		break;
	case CallbackType::PERSONAL_STATUS:
		rc = sqlite3_exec(db, s.str().c_str(), callbackPersonStatus, &_personalStatus, &zErrMsg);
		break;
	case CallbackType::PLAYERS_ANSWERS:
		rc = sqlite3_exec(db, s.str().c_str(), callbackCount, &_playersAnswers, &zErrMsg);
	default:
		break;
	}
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL error: " << zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	s.str(std::string());
	s.clear();
}

Question* DataBase::getQuestionFromMap(int index)
{
	int id = std::stoi(_questions.find("question_id")->second[index]);
	std::string question = _questions.find("question")->second[index];
	std::string correctAns = _questions.find("correct_ans")->second[index];
	std::string ans2 = _questions.find("ans2")->second[index];
	std::string ans3 = _questions.find("ans3")->second[index];
	std::string ans4 = _questions.find("ans4")->second[index];
	Question* q = new Question(id, question, correctAns, ans2, ans3, ans4);
	return q;
}

int DataBase::getMaxElementIndex(std::unordered_map<std::string, std::vector<std::string>>& map)
{
	//Makes a vector from the values of the map
	std::vector<int> vec;
	auto it = map.find("sum(is_correct)");
	if (it != map.end())
		for (int i = 0; i < it->second.size(); i++)
			vec.push_back(std::stoi(it->second[i]));

	//Returns the index of the largest element in the created vector
	int index_max = 0;
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] > vec[index_max])
			index_max = i;
	return index_max;
}