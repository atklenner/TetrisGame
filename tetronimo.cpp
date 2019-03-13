#include "tetronimo.h"
#include <QtCore>

//Constructor for a tetris piece, sets it to a default non-piece.
Tetronimo::Tetronimo() {
    setShape(NoShape);
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
            coords[i][j] = coordsTable[shape][i][j];
    }
    pieceShape = shape;
}

//Simply returns the shape.
TetronimoShape Tetronimo::shape() const {
    return pieceShape;
}

int Tetronimo::x(int index) const {
    return coords[index][0];
}

int Tetronimo::y(int index) const {
    return coords[index][1];
}

int Tetronimo::minX() const {
    int min = coords[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][0]);
    return min;

}

int Tetronimo::maxX() const {
    int max = coords[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][0]);
    return max;
}

int Tetronimo::minY() const {
    int min = coords[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][1]);
    return min;
}

int Tetronimo::maxY() const {
    int max = coords[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][1]);
    return max;
}

Tetronimo Tetronimo::rotatedLeft() const {
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

Tetronimo Tetronimo::rotatedRight() const {
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
    coords[index][0] = x;
}

void Tetronimo::setY(int index, int y) {
    coords[index][1] = y;
}
