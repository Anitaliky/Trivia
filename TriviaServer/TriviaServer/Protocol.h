#define DEFAULT_PORT "8820"

#define SUCCESS "0"
#define FAIL1 "1"
#define FAIL2 "2"
#define FAIL3 "3"
#define FAIL4 "4"

#define CORRECT_ANSWER "1"
#define INCORRECT_ANSWER "0"

typedef enum class client_message_code : int
{
	SIGN_IN = 200,
	SIGN_OUT = 201,
	SIGN_UP = 203,
	ROOM_LIST = 205,
	USERS_ROOM_LIST = 207,
	JOIN_ROOM = 209,
	LEAVE_ROOM = 211,
	CREATE_ROOM = 213,
	CLOSE_ROOM = 215,
	START_GAME = 217,
	ANSWER = 219,
	LEAVE_GAME = 222,
	BEST_SCORES = 223,
	PERSONAL_STATUS = 225,
	EXIT = 299
} ClientMessageCode;

typedef enum class server_message_code : int
{
	SIGN_IN = 102,
	SIGN_UP = 104,
	ROOM_LIST = 106,
	USERS_ROOM_LIST = 108,
	JOIN_ROOM = 110,
	LEAVE_ROOM = 112,
	CREATE_ROOM = 114,
	CLOSE_ROOM = 116,
	QUESTION = 118,
	ANSWER_CORRECTNESS = 120,
	END_GAME = 121,
	BEST_SCORES = 124,
	PERSONAL_STATUS = 126
} ServerMessageCode;