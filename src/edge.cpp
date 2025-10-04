#include "edge.h"
#include "vertex.h"

void Edge::SetState(State state)
{
    if (!a_ || !b_)
        return;
    // First change active edge of its connected vertices
    a_->ChangeActiveEdge(state_, state);
    b_->ChangeActiveEdge(state_, state);
    // Then change state
    state_ = state;
};