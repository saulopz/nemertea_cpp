#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

enum class State
{
    NONE = 0,
    CONQUERED = 2,
    INACTIVE = 3
};

std::string stateToString(State state);

class Vertex;

class Vertex
{
  private:
    uint64_t id_;                     // Vertex id
    std::string name_;                // Vertex name or description
    double x_, y_;                    // Vertex position on 2D space
    Vertex *parent_;                  // Vertex parent in NBFS tree
    std::vector<Vertex *> neighbors_; // List of connected neighboring vertices
    State state_;                     // Vertex state
    uint64_t generation_;             // If the search_id_ in NBFS is equal,
                                      //    the vertex is currently being tested.

  public:
    Vertex(const uint64_t id, std::string name, const double x, const double y)
        : id_(id), name_(std::move(name)), x_(x), y_(y), parent_(nullptr), neighbors_(),
          state_(State::NONE), generation_(0) {};

    void Connect(Vertex *target);

    ~Vertex() {};

    [[nodiscard]] uint64_t GetId() const
    {
        return id_;
    };

    [[nodiscard]] uint64_t GetCurrentSearchId() const
    {
        return generation_;
    }

    [[nodiscard]] Vertex *GetParent() const
    {
        return parent_;
    }

    void AddToGeneration(Vertex *parent, uint64_t generation)
    {
        parent_ = parent;
        generation_ = generation;
    }

    [[nodiscard]] bool IsTesting(uint64_t search_id)
    {
        return generation_ == search_id;
    }

    [[nodiscard]] const std::string &GetName() const
    {
        return name_;
    };

    [[nodiscard]] double GetX() const
    {
        return x_;
    };

    [[nodiscard]] double GetY() const
    {
        return y_;
    };

    [[nodiscard]] State GetState() const
    {
        return state_;
    };

    [[nodiscard]] size_t GetNeighborsCount() const
    {
        return neighbors_.size();
    };

    [[nodiscard]] Vertex *GetNeighbor(size_t i) const
    {
        return neighbors_[i];
    }

    void SetState(const State state)
    {
        state_ = state;
    };
};

#endif // VERTEX_HPP_
