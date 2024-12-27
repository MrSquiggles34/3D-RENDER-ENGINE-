#ifndef NODE_H
#define NODE_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>

#include "BallObject.h"

extern int MAX_LVL;

class Node {
public:
    static std::queue<Node*> NodePool;

    Node(int _x, int _y, int _w, int _h, int _lvl)
        : x(_x), y(_y), w(_w), h(_h), lvl(_lvl) {}

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
        children.clear();
    }

    static Node* Create(int _x, int _y, int _w, int _h, int _lvl);

    void Add(BallObject* _entity);
    int Inside(BallObject* _entity);
    void Set(int _x, int _y, int _w, int _h, int _lvl);
    void DetectCollisions();

private:
    int x, y;
    int w, h;
    int lvl;

    std::vector<Node*> children;
    std::vector<BallObject*> entities;
};

#endif
