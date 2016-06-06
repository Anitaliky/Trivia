#include "Game.h"

Game::Game(const std::vector<User*>& players, int questionsNo, DataBase& db) : _db(db)
{
	_questions_no = questionsNo;
	_id = _db.insertNewGame();
	if (_id == INVALID_ID)
		throw std::exception("Game insert failed!");
	else
		initQuestionsFromDB();
	_players = players;
	for (int i = 0; i < _players.size(); i++)
	{
		std::pair<std::string, int> result;
		result.first = _players[i]->getUsername();
		result.second = 0;
		_results.insert(result);

		_players[i]->setGame(this);
	}
	_currQuestionIndex = 0;
}

Game::~Game()
{
	for (int i = 0; i < _questions.size(); i++)
		delete _questions[i];
	_questions.clear();
	for (int i = _players.size() - 1; i >= 0 ; i--)
		_players.pop_back();
}

void Game::handleFinishGame()
{
	if (_db.updateGameStatus(_id))
	{
		std::string message = std::to_string((int)ServerMessageCode::END_GAME) + Helper::getPaddedNumber(_players.size(), 1);
		for (int i = 0; i < _players.size(); i++)
			message += Helper::getPaddedNumber(_players[i]->getUsername().size(), 2) + _players[i]->getUsername() + Helper::getPaddedNumber(std::stoi(_db.getScoreByUsername(_players[i]->getUsername())), 2);
		for (int i = 0; i < _players.size(); i++)
		{
			try
			{
				_players[i]->send(message);
			}
			catch (std::exception& ex)
			{
				std::cout << ex.what() << std::endl;
			}
			_players[i]->setGame(nullptr);
		}
	}
}

void Game::sendFirstQuestion()
{
	sendQuestionToAllUsers();
}

bool Game::handleNextTurn()
{
	if (_players.size())
	{
		if (_currentTurnAnswers == _players.size())
		{
			if (_currQuestionIndex == _questions.size() - 1)
				handleFinishGame();
			else
			{
				_currQuestionIndex++;
				sendQuestionToAllUsers();
			}
		}
		return true;
	}
	handleFinishGame();
	return false;
}

bool Game::handleAnswerFromUser(User* user, int answerNo, int time)
{
	bool isCorrect = false;
	_currentTurnAnswers++;
	if (answerNo == _questions[_currQuestionIndex]->getCorrectAnswerIndex() + 1)
	{
		isCorrect = true;
		_results.at(user->getUsername())++;
	}
	std::string answer = answerNo != 5 ? _questions[_currQuestionIndex]->getAnswers()[answerNo] : "";
	_db.addAnswerToPlayer(_id, user->getUsername(), _questions[_currQuestionIndex]->getId(), answer, isCorrect, time);
	std::string message = std::to_string((int)ServerMessageCode::ANSWER_CORRECTNESS) + std::to_string(isCorrect);
	Helper::sendData(user->getSocket(), message);
	return handleNextTurn();
}

bool Game::leaveGame(User* currUser)
{
	bool flag = false;
	for (int i = 0; i < _players.size() && !flag; i++)
		if (_players[i] == currUser)
		{
			_players.erase(_players.begin() + i);
			flag = true;
		}
	return handleNextTurn();
}

int Game::getID()
{
	return _id;
}

//private functions
bool Game::insertGameToDB()
{
	return _db.insertNewGame() != INVALID_ID;
}

void Game::initQuestionsFromDB()
{
	_questions = _db.initQuestions(_questions_no);
}

void Game::sendQuestionToAllUsers() throw(...)
{
	std::string question = _questions[_currQuestionIndex]->getQuestion();
	std::string* answers = _questions[_currQuestionIndex]->getAnswers();
	std::string message = std::to_string((int)(ServerMessageCode::QUESTION)) + Helper::getPaddedNumber(question.length(), 3) + question;
	if (question.size())
		for (int i = 0; i < 4; i++)
			message += Helper::getPaddedNumber(answers[i].size(), 3) + answers[i];
	_currentTurnAnswers = 0;
	for (int i = 0; i < _players.size(); i++)
	{
		try
		{
			_players[i]->send(message);
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
}