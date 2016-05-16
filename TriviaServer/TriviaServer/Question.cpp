#include "Question.h"
#include <ctime>

Question::Question(int id, std::string question, std::string correctAnswer, std::string answer2, std::string answer3, std::string answer4)
{
	int j = 0;
	std::string nonCorrectAnswers[3] = { answer2, answer3, answer4 };
	std::srand(std::time(0));
	_correctAnswerIndex = std::rand() % 4 + 0;
	for (int i = 0; i < 4; i++)
	{
		if (i == _correctAnswerIndex)
			_answers[i] = correctAnswer;
		else
		{
			_answers[i] = nonCorrectAnswers[j];
			j++;
		}
	}

	_question = question;
	_id = id;
}

std::string Question::getQuestion()
{
	return _question;
}

std::string* Question::getAnswers()
{
	return _answers;
}

int Question::getCorrectAnswerIndex()
{
	return _correctAnswerIndex;
}

int Question::getId()
{
	return _id;
}