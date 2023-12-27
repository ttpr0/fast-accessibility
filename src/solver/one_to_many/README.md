# documentation

This directory contains usefull classes to efficiently solve one-to-many problems.

Solvers have the following structure:

```cpp
// solves one-to-many shortest-path problems
class Solver
{
public:
    void addMaxRange(int range); // add maximum computed range (neccessary for some algorithms)
    void addTarget(int id); // add target to search
    
    bool isBuild(); // check if Solver is build
    void build(); // build Solver, only use addMaxRange and addTarget before and compute after build

    State makeComputeState(); // creates a new compute state
    void compute(int start, State& state); // computes batched-shortest-paths to all targets
};

// stores relevant information for every one-to-many iteration.
// can be reused by passing it to compute multiple times
class State
{
public:
    int getDistance(int id); // returns the computed distance to the target node with id
};
```
