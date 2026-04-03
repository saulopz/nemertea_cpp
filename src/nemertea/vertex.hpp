#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

/**
 * Represents the tree states of Vertex and Edges Vertex has 2 States:
 * - NONE: Inactive vertex
 * - CONQUERED: It's part of Nemertea frontear
 *
 * And Edges is represented as a array V x V to a bidimensional matrix of
 * adjacense of vertices conection. It has tree states possible:
 * - NONE: Not connected
 * - CONQUERED: It's part of Nemertea frontear
 * - INACTIVE: It's connected but not on Nemertea
 *             frontear
 */
enum class State
{
    NONE = 0,
    CONQUERED = 2,
    INACTIVE = 3
};

/**
 * Returns the state on string format.
 *
 * @param state The state as defined in cass State
 * @return String represents the State.
 */
std::string stateToString(State state);

/**
 * Represents a vertex 'v' of a graph G(V, E) | v ∈ V.
 *
 * Stores incident edges and logical state used by the Nemertea
 * and NBFS algorithms.
 *
 * This class is part of the Nemertea project.
 * Created by Saulo Popov Zambiasi, 2025.
 *
 * Attributes
 * ----------
 * - id_        : identifier of a unique vertex.
 * - name_      : vertex name or description.
 * - x_, y_     : spatial position of vertex.
 * - parent_    : parent of this vertex in the search tree
 * - neighbors_ : list of vertices connected
 * - state_     : state of vertex (NONE | CONQUERED)
 * - generation_: id of iteration. If g = Proboscis(g), this vertex
 *                is on testing state.
 */
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
    /**
     * Constructs a new Vertex with the given attributes.
     *
     * @param id Unique identifier for the vertex.
     * @param name Name or description of the vertex.
     * @param x X-coordinate of the vertex in 2D space.
     * @param y Y-coordinate of the vertex in 2D space.
     */
    Vertex(const uint64_t id, std::string name, const double x, const double y)
        : id_(id), name_(std::move(name)), x_(x), y_(y), parent_(nullptr),
          neighbors_(), state_(State::NONE), generation_(0) {};

    /**
     * Connects this vertex to another vertex, creating an undirected edge
     * between them.
     * @param target The vertex to connect to.
     */
    void Connect(Vertex *target);

    /**
     * Returns the unique identifier of this vertex.
     * @return The vertex ID.
     */
    [[nodiscard]] uint64_t GetId() const
    {
        return id_;
    };

    /**
     * Returns the current search ID associated with this vertex. If this
     * ID matches the search ID used in the NBFS algorithm, it indicates that
     * this vertex is currently being tested.
     * @return The generation ID of this vertex.
     */
    [[nodiscard]] uint64_t GetGeneration() const
    {
        return generation_;
    }

    /**
     * Returns the parent vertex of this vertex in the search tree.
     * @return The parent vertex of this vertex.
     */
    [[nodiscard]] Vertex *GetParent() const
    {
        return parent_;
    }

    /**
     * Sets the parent vertex and generation ID for this vertex, indicating
     * that it is currently being tested in the NBFS algorithm.
     * @param parent The parent vertex in the search tree.
     * @param generation The generation ID to associate with this vertex.
     */
    void AddToGeneration(Vertex *parent, uint64_t generation)
    {
        parent_ = parent;
        generation_ = generation;
    }


    /**
     * Checks if this vertex is currently being tested in the NBFS algorithm.
     * @param generation The generation ID to compare against this vertex's generation ID.
     * @return True if this vertex is being tested (generation IDs match), false otherwise.
     */
    [[nodiscard]] bool IsTesting(uint64_t generation)
    {
        return generation_ == generation;
    }

    /**
     * Returns the name of this vertex.
     * @return The name of this vertex.
     */
    [[nodiscard]] const std::string &GetName() const
    {
        return name_;
    };

    /**
     * Returns the X-coordinate of this vertex in 2D space.
     * @return The X-coordinate of this vertex.
     */
    [[nodiscard]] double GetX() const
    {
        return x_;
    };

    /**
     * Returns the Y-coordinate of this vertex in 2D space.
     * @return The Y-coordinate of this vertex.
     */
    [[nodiscard]] double GetY() const
    {
        return y_;
    };

    /**
     * Returns the state of this vertex (NONE | CONQUERED).
     * @return The state of this vertex.
     */
    [[nodiscard]] State GetState() const
    {
        return state_;
    };

    /**
     * Returns the number of neighbors of this vertex.
     * @return The number of neighbors of this vertex.
     */
    [[nodiscard]] size_t GetNeighborsCount() const
    {
        return neighbors_.size();
    };

    /**
     * Returns the neighbor vertex at the specified index.
     * @param i The index of the neighbor to retrieve.
     * @return The neighbor vertex at the specified index.
     */
    [[nodiscard]] Vertex *GetNeighbor(size_t i) const
    {
        return neighbors_[i];
    }

    /**
     * Sets the state of this vertex.
     * @param state The state to set for this vertex.
     */
    void SetState(const State state)
    {
        state_ = state;
    };
};

#endif // VERTEX_HPP_
