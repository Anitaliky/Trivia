#include "Validator.h"

bool Validator::isPasswordValid(std::string password)
{
	return (password.length() >= 4 &&
		password.find_first_of(' ') == std::string::npos &&
		password.find_first_of("1234567890") != std::string::npos &&
		password.find_first_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos &&
		password.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos
		);
}

bool Validator::isUsernameValid(std::string username)
{
	return (username != "" &&
		(username.at(0) >= 'a' && username.at(0) <= 'z') || (username.at(0) >= 'A' && username.at(0) <= 'Z') &&
		username.find_first_of(' ') == std::string::npos);
}