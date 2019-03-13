#ifndef TETRONIMO_H
#define TETRONIMO_H

//This simplifies the way we refer to a specific shape, instead of
//using an int we have a more descriptive name for the int.
enum TetronimoShape { NoShape, ZShape, SShape, IShape, TShape, OShape,
                   LShape, JShape };

class Tetronimo
{
public:
    //Constructs tetris piece.
    Tetronimo();

    //Sets the piece to a random shape.
    void setRandomShape();

    //Sets piece to a specific shape.
    void setShape(TetronimoShape shape);

    //Returns the shape of the piece.
    TetronimoShape shape() const;

    //
    int x(int index) const;
    int y(int index) const;
    int minX() const;
    int maxX() const;
    int minY() const;
    int maxY() const;
    Tetronimo rotatedLeft() const;
    Tetronimo rotatedRight() const;

private:
    void setX(int index, int x);
    void setY(int index, int y);

    TetronimoShape pieceShape;
    int coords[4][2];
};

#endif // TETRONIMO_H
