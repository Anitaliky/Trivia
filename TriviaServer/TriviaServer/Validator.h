#pragma once
#include <iostream>
#include <string>
#include <cctype>

class Validator
{
public:
	static bool isPasswordValid(std::string);
	static bool isUsernameValid(std::string);
};