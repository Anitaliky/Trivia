#pragma once
#include <iostream>
#include <string>

class Question
{
public:
	Question(int, std::string, std::string, std::string, std::string, std::string);
	std::string getQuestion();
	std::string* getAnswers();
	int getCorrectAnswerIndex();
	int getId();

private:
	std::string _question;
	std::string _answers[4];
	int _correctAnswerIndex;
	int _id;
};