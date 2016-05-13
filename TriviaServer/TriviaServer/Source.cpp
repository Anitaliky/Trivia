#include "Validator.h"
#include "TriviaServer.h"

void main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	struct addrinfo* result;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	try
	{
		TriviaServer ts;
		ts.createSocket();
		std::cout << "Waiting for clients to connect..." << std::endl;
		ts.serve(result);
	}
	catch (std::exception& ex)
	{
		printException(ex);
	}
}

//this function is called in 'catch' in the try-catch block
void printException(std::exception e)
{
	std::cout << e.what() << std::endl;
	WSACleanup();
	system("pause");
}