#include "vertex.hpp"

void Vertex::Connect(Vertex *target)
{
    neighbors_.push_back(target);
};

void Vertex::ChangeActiveConnection(State current_state, State new_state)
{
    if (current_state != State::ACTIVE && new_state == State::ACTIVE)
    {
        active_connection_count_++;
        return;
    }
    if (current_state == State::ACTIVE && new_state != State::ACTIVE)
        active_connection_count_--;
}

std::string stateToString(const State state)
{
    switch (state)
    {
    case State::NONE:      // Not connected
        return "NONE";     //
    case State::INACTIVE:  // Connected but inactive
        return "INACTIVE"; //
    case State::ACTIVE:    // Connected and active
        return "ACTIVE";   //
    case State::TESTING:   // Connected and testing
        return "TESTING";  //
    default:               //
        return "UNKNOWN";  // Some error
    }
}