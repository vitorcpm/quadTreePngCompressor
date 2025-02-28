#include <iostream>
#include <cassert>
#include <fstream>
#include "PNGImage.h"
#include "QuadTree.h"

using namespace std;

int main(int argc, char **argv) 
{
    if (argc != 4) 
    {
        cerr << "Incorrect input. Expected format:\n"
             << "-----------------------------------------\n"
             << argv[0] << " compress <input.png> <compressed.txt>\n"
             << "-----------------------------------------\n"
             << argv[0] << " decompress <compressed.txt> <output.png>\n"
             << "-----------------------------------------" << endl;
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    if (mode == "compress") 
    {
        PNGImage image;
        if (!image.loadFromFile(inputFile)) 
        {
            cerr << "Error loading image: " << inputFile << endl;
            return 1;
        }

        QuadTree *qt = new QuadTree();
        qt->node = QuadTree::buildCompressedQuadTree(image, 0, 0, image.getWidth(), 10);

        string compressedData = qt->quadTreeToString(qt->node);

        ofstream outFile(outputFile);
        if (!outFile) 
        {
            cerr << "Error opening output file: " << outputFile << endl;
            return 1;
        }

        outFile << image.getWidth() << " " << image.getHeight() << endl;
        outFile << compressedData << endl;
        outFile.close();

        delete qt;
    } 
    else if (mode == "decompress") 
    {
        ifstream inFile(inputFile);
        if (!inFile) 
        {
            cerr << "Error opening compressed file: " << inputFile << endl;
            return 1;
        }

        int size, maxIntensity;
        inFile >> size >> maxIntensity;
        inFile.ignore();

        string compressedQuadTreeStr;
        getline(inFile, compressedQuadTreeStr);
        inFile.close();

        int pos = 0;
        QuadTree *qt = new QuadTree();
        qt->node = QuadTree::rebuildCompressedQuadTree(compressedQuadTreeStr, pos);

        PNGImage image(size, size);
        qt->rebuildImage(qt->node, image, 0, 0, size);

        if (!image.saveToFile(outputFile)) 
        {
            cerr << "Error saving decompressed image: " << outputFile << endl;
            return 1;
        }

        delete qt;
    } 
    else 
    {
        cerr << "Invalid mode '" << mode << "', use: 'compress' or 'decompress'" << endl;
        return 1;
    }

    return 0;
}
