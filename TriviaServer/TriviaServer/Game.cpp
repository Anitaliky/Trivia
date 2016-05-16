#include "Game.h"

Game::Game(const std::vector<User*>& players, int questionsNo, DataBase& db, int id) : _db(db)
{
	//_db.insertNewGame();	//TO ADD: exception
	//_questions = _db.initQuestions(questionsNo);
	_players = players;
	_questions_no = questionsNo;
	for (int i = 0; i < _players.size(); i++)
	{
		std::pair<std::string, int> result;
		result.first = _players[i]->getUsername();
		result.second = 0;
		_results.insert(result);

		_players[i]->setGame(this);
	}
	_currQuestionIndex = 0;
	_id = id;
}

Game::~Game()
{
	for (int i = _questions.size() - 1; i >= 0; i--)
	{
		_questions.pop_back();
	}
	for (int i = _players.size() - 1; i >= 0; i--)
		_players.pop_back();
}

void Game::handleFinishGame()
{

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
	std::string isCorrect = INCORRECT_ANSWER;
	_currentTurnAnswers++;
	if (answerNo == _questions[_currQuestionIndex]->getCorrectAnswerIndex())
	{
		_results.at(user->getUsername())++;
		//_db.addAnswerToPlayer(...);
		isCorrect = CORRECT_ANSWER;
	}
	std::string message = std::to_string((int)ServerMessageCode::ANSWER_CORRECTNESS) + isCorrect;
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
	return true;//TODO
}

void Game::initQuestionsFromDB()
{

}

void Game::sendQuestionToAllUsers() throw(...)
{
	std::string question = _questions[_currQuestionIndex]->getQuestion();
	std::string* answers = _questions[_currQuestionIndex]->getAnswers();
	std::string message = std::to_string((int)(ServerMessageCode::QUESTION)) + std::to_string(question.length()) + question;
	if (question.length())
		for (int i = 0; i < answers->length(); i++)
			message += std::to_string(answers[i].length()) + answers[i];
	_currentTurnAnswers = 0;
	for (int i = 0; i < _players.size(); i++)
	{
		try
		{
			_players[i]->send(message);
		}
		catch (std::exception ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
}