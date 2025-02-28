#ifndef QUADTREE_H
#define QUADTREE_H

#include "PNGImage.h"

// Defines the QuadTree node structure
struct QuadTreeNode 
{
    bool isLeaf;
    Color color;
    
    QuadTreeNode *q1, *q2, *q3, *q4; // Quadrants: Top-Left, Top-Right, Bottom-Left, Bottom-Right

    QuadTreeNode(const Color &c)
        : isLeaf(true), color(c), q1(nullptr), q2(nullptr), q3(nullptr), q4(nullptr) {}

    QuadTreeNode(QuadTreeNode *q1_, QuadTreeNode *q2_, QuadTreeNode *q3_, QuadTreeNode *q4_)
        : isLeaf(false), q1(q1_), q2(q2_), q3(q3_), q4(q4_) {}

    ~QuadTreeNode() 
    {
        delete q1;
        delete q2;
        delete q3;
        delete q4;
    }
};

class QuadTree 
{
public:
    QuadTreeNode *node;

    QuadTree() : node(nullptr) {}
    ~QuadTree() { delete node; }

    static QuadTreeNode *buildCompressedQuadTree(const PNGImage &image, int x, int y, int size, int tolerance);
    std::string quadTreeToString(const QuadTreeNode *node) const;
    static QuadTreeNode *rebuildCompressedQuadTree(const std::string &data, int &pos);
    void rebuildImage(const QuadTreeNode *node, PNGImage &image, int x, int y, int size) const;
};

#endif
