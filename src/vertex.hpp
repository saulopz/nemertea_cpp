#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

enum class State
{
    NONE = 0,
    INACTIVE = 1,
    ACTIVE = 2,
    TESTING = 3
};

std::string stateToString(State state);

class Vertex;

class Vertex
{
  private:
    Vertex *parent_;                  // Parent in NBFS tree
    uint64_t id_;                     // Vertex id
    std::string name_;                // Vertex name or description
    double x_, y_;                    // Vertex position on 2D space
    size_t active_connection_count_;  // Number of active edges
    std::vector<Vertex *> neighbors_; // List of edges connected
    State state_;                     // Vertex state (NONE, ACTIVE, TESTING)
    uint64_t search_id_;              // Used in NBFS
  public:
    Vertex(const uint64_t id, std::string name, const double x, const double y)
        : id_(id), name_(std::move(name)), x_(x), y_(y), active_connection_count_(0), neighbors_(), state_(State::NONE),
          search_id_(0)
    {
    }

    void ChangeActiveConnection(State current_state, State new_state);
    void Connect(Vertex *target);
    ~Vertex() {};

    [[nodiscard]] uint64_t GetId() const
    {
        return id_;
    };

    [[nodiscard]] uint64_t GetCurrentSearchId() const
    {
        return search_id_;
    }

    [[nodiscard]] Vertex *GetParent() const
    {
        return parent_;
    }

    void AddToSearch(Vertex *parent, uint64_t search_id)
    {
        parent_ = parent;
        search_id_ = search_id;
    }

    [[nodiscard]] bool IsTesting(uint64_t search_id)
    {
        return search_id_ == search_id;
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

    void AddActiveConnections()
    {
        active_connection_count_++;
    }

    void DecActiveConnections()
    {
        active_connection_count_--;
    }

    [[nodiscard]] size_t GetActiveConnectionsCount() const
    {
        return active_connection_count_;
    };
};

#endif // VERTEX_HPP_
