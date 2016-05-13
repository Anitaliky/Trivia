#pragma once
#include <iostream>
#include <string>

class Validator
{
public:
	static bool isPasswordValid(std::string);
	static bool isUsernameValid(std::string);
};