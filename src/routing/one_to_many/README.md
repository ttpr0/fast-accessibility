# documentation

This directory contains usefull classes to efficiently solve one-to-many problems.

Solvers have the following structure:

```cpp
class Solver
{
public:
    typedef ... Graph;
    class Builder
    {
    public:
        Builder(Graph g); // create solver-builder
        void addMaxRange(int range); // add maximum computed range (neccessary for some algorithms)
        void addTarget(int id); // add target to search
        Solver build(); // build solver
    }

    compute(int start, DistFlagArray& flags); // computes batched-shortest-paths to all targets
}
```
