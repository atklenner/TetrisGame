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

    //Returns the x or y coordinate at a given index.
    int x(int) const;
    int y(int) const;

    //Returns the max or min x/y coordinate for all indeces.
    int minX() const;
    int maxX() const;
    int minY() const;
    int maxY() const;

    //Sets the piece to a random shape.
    void setRandomShape();

    //Sets piece to a specific shape.
    void setShape(TetronimoShape);

    //Returns the shape of the piece.
    TetronimoShape shape() const;

    //Rotates the tetronimo by multiplying by a rotation matrix.
    Tetronimo rotateLeft() const;
    Tetronimo rotateRight() const;

private:

    //Sets the x or y coordinate.
    void setX(int, int);
    void setY(int, int);

    //Stores the descriptive name of the shape.
    TetronimoShape pieceShape;

    //This two dimensional array stores where the four blocks of every
    //tetronimo are placed, based on the pieceShape.
    int coordinates[4][2];
};

#endif // TETRONIMO_H
