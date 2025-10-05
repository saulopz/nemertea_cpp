#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <string>

enum class State { NONE = 0, TESTING = 1, ACTIVE = 2 };

std::string stateToString(State state);

#endif // GLOBALS_H_
