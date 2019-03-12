#ifndef TETRONIMO_H
#define TETRONIMO_H

enum TetronimoShape { NoShape, ZShape, SShape, LineShape, TShape, SquareShape,
                   LShape, MirroredLShape };


class Tetronimo
{
public:
    Tetronimo() { setShape(NoShape); }

    void setRandomShape();
    void setShape(TetronimoShape shape);

    TetronimoShape shape() const { return pieceShape; }
    int x(int index) const { return coords[index][0]; }
    int y(int index) const { return coords[index][1]; }
    int minX() const;
    int maxX() const;
    int minY() const;
    int maxY() const;
    Tetronimo rotatedLeft() const;
    Tetronimo rotatedRight() const;

private:
    void setX(int index, int x) { coords[index][0] = x; }
    void setY(int index, int y) { coords[index][1] = y; }

    TetronimoShape pieceShape;
    int coords[4][2];
};

#endif // TETRONIMO_H
