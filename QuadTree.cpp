#include "QuadTree.h"
#include <sstream>

using namespace std;

QuadTreeNode* QuadTree::buildCompressedQuadTree(const PNGImage &image, int x, int y, int size, int tolerance) 
{
    Color representativeColor;

    if (image.isUniform(x, y, size, representativeColor, tolerance)) 
    {
        return new QuadTreeNode(representativeColor);
    } 
    else 
    {
        int halfSize = size / 2;

        QuadTreeNode *q1 = buildCompressedQuadTree(image, x, y, halfSize, tolerance);
        QuadTreeNode *q2 = buildCompressedQuadTree(image, x + halfSize, y, halfSize, tolerance);
        QuadTreeNode *q3 = buildCompressedQuadTree(image, x, y + halfSize, halfSize, tolerance);
        QuadTreeNode *q4 = buildCompressedQuadTree(image, x + halfSize, y + halfSize, halfSize, tolerance);

        return new QuadTreeNode(q1, q2, q3, q4);
    }
}

string QuadTree::quadTreeToString(const QuadTreeNode *node) const 
{
    if (node->isLeaf) 
    {
        return "(" + to_string(node->color.r) + "," + to_string(node->color.g) + "," + to_string(node->color.b) + ")";
    } 
    else 
    {
        return "(" + quadTreeToString(node->q1) + quadTreeToString(node->q2) + 
               quadTreeToString(node->q3) + quadTreeToString(node->q4) + ")";
    }
}

QuadTreeNode* QuadTree::rebuildCompressedQuadTree(const string &data, int &pos) 
{
    if (data[pos] == '(') 
    {
        pos++;

        if (data[pos] == '(') 
        {
            QuadTreeNode* q1 = rebuildCompressedQuadTree(data, pos);
            QuadTreeNode* q2 = rebuildCompressedQuadTree(data, pos);
            QuadTreeNode* q3 = rebuildCompressedQuadTree(data, pos);
            QuadTreeNode* q4 = rebuildCompressedQuadTree(data, pos);
            
            pos++; 

            return new QuadTreeNode(q1, q2, q3, q4);
        } 
        else 
        {
            int r, g, b;
            int start = pos;

            while (data[pos] != ')' && data[pos] != '(') 
            {
                pos++;
            }

            istringstream colorStr(data.substr(start, pos - start));
            char comma;

            colorStr >> r >> comma >> g >> comma >> b;
            
            pos++; 

            return new QuadTreeNode(Color(r, g, b));
        }
    }
    return nullptr;
}

void QuadTree::rebuildImage(const QuadTreeNode *node, PNGImage &image, int x, int y, int size) const 
{
    if (node->isLeaf) 
    {
        for (int i = y; i < y + size; i++) 
        {
            for (int j = x; j < x + size; j++) 
            {
                image.set(j, i, node->color);
            }
        }
    } 
    else 
    {
        int halfSize = size / 2;

        rebuildImage(node->q1, image, x, y, halfSize);
        rebuildImage(node->q2, image, x + halfSize, y, halfSize);
        rebuildImage(node->q3, image, x, y + halfSize, halfSize);
        rebuildImage(node->q4, image, x + halfSize, y + halfSize, halfSize);
    }
}