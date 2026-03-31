#include "vertex.hpp"

void Vertex::Connect(Vertex *target)
{
    neighbors_.push_back(target);
};

std::string stateToString(const State state)
{
    switch (state)
    {
    case State::NONE:       // Not connected
        return "NONE";      //
    case State::CONQUERED:  // Connected and active
        return "CONQUERED"; //
    case State::INACTIVE:   // Connected but inactive
        return "INACTIVE";  //
    default:                //
        return "UNKNOWN";   // Some error
    }
}