#include "tetronimo.h"
#include <QtCore>

//Constructor for a tetris piece, sets it to a default non-piece.
Tetronimo::Tetronimo() {
    setShape(NoShape);
}

int Tetronimo::x(int index) const {
    return coordinates[index][0];
}

int Tetronimo::y(int index) const {
    return coordinates[index][1];
}

int Tetronimo::minX() const {
    int min = coordinates[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coordinates[i][0]);
    return min;

}

int Tetronimo::maxX() const {
    int max = coordinates[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coordinates[i][0]);
    return max;
}

int Tetronimo::minY() const {
    int min = coordinates[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coordinates[i][1]);
    return min;
}

int Tetronimo::maxY() const {
    int max = coordinates[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coordinates[i][1]);
    return max;
}

//Sets the piece to a random shape using a random number generator.
void Tetronimo::setRandomShape() {
    setShape(TetronimoShape(QRandomGenerator::global()->bounded(7) + 1));
}

//Sets the shape by using a lookup table where each row corresponds to
//a piece shape. Each row contains four coordinates which represent the
//four blocks that make up a tetronimo. The (0,0) is the block which the
//piece rotates around.
void Tetronimo::setShape(TetronimoShape shape) {
    static const int coordsTable[8][4][2] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }
    };

    for (int i = 0; i < 4 ; i++) {
        for (int j = 0; j < 2; ++j)
            coordinates[i][j] = coordsTable[shape][i][j];
    }
    pieceShape = shape;
}

//Simply returns the shape.
TetronimoShape Tetronimo::shape() const {
    return pieceShape;
}



Tetronimo Tetronimo::rotateLeft() const {
    if (pieceShape == OShape)
        return *this;

    Tetronimo result;
    result.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, y(i));
        result.setY(i, -x(i));
    }
    return result;
}

Tetronimo Tetronimo::rotateRight() const {
    if (pieceShape == OShape)
        return *this;

    Tetronimo result;
    result.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, -y(i));
        result.setY(i, x(i));
    }
    return result;
}

void Tetronimo::setX(int index, int x) {
    coordinates[index][0] = x;
}

void Tetronimo::setY(int index, int y) {
    coordinates[index][1] = y;
}
