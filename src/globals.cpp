#include "globals.h"

std::string stateToString(State state)
{
    switch (state)
    {
    case State::ACTIVE:
        return "ACTIVE";
    case State::NONE:
        return "INACTIVE";
    case State::TESTING:
        return "TESTING";
    default:
        return "UNKNOWN";
    }
};