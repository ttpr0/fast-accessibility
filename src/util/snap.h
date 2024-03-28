#pragma once

struct Snap
{
    int node;
    int dist;
};

struct DSnap
{
    Snap snaps[2];

    DSnap()
    {
        this->snaps[0] = {-1, 0};
        this->snaps[1] = {-1, 0};
    }
    DSnap(int node)
    {
        this->snaps[0] = {node, 0};
        this->snaps[1] = {-1, 0};
    }
    DSnap(Snap snap)
    {
        this->snaps[0] = snap;
        this->snaps[1] = {-1, 0};
    }
    DSnap(Snap snap1, Snap snap2)
    {
        this->snaps[0] = snap1;
        this->snaps[1] = snap2;
    }

    int len() const
    {
        if (snaps[0].node == -1) {
            return 0;
        }
        if (snaps[1].node == -1) {
            return 1;
        }
        return 2;
    }

    Snap operator[](int i) const { return this->snaps[i]; }
};
