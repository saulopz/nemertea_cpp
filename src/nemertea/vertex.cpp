#include "vertex.hpp"

void Vertex::Connect(Vertex *target)
{
    neighbors_.push_back(target);
};

std::string stateToString(const State state)
{
    switch (state)
    {
    case State::NONE:       //
        return "NONE";      // Not connected
        [[fallthrough]];    //
    case State::CONQUERED:  //
        return "CONQUERED"; // Connected and active
        [[fallthrough]];    //
    case State::INACTIVE:   //
        return "INACTIVE";  // Connected but inactive
        [[fallthrough]];    //
    default:                //
        return "UNKNOWN";   // Some error
    }
}
