#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QBasicTimer>
#include <QFrame>
#include <QPointer>

#include "tetronimo.h"

class QLabel;

class GameBoard : public QFrame
{
    Q_OBJECT

public:

    GameBoard(QWidget *parent = 0);

    //Set the labels on the widget depending on what piece is next and what
    //is stored in holdPiece.
    void setNextPieceLabel(QLabel *label);
    void setHoldPieceLabel(QLabel *label);

public slots:

    //Starts the game when the start button is pressed in the widget.
    void start();

    //Pauses the game when the button is pressed or the P key is pressed.
    void pause();

    //Modifies the difficulty by speeding up the game by and integer value,
    //this is affected by a horizontal slider in the widget.
    void difficulty(int);

signals:

    //These update QLabels on the widget.
    void scoreChange(int);
    void levelChange(int);
    void linesRemovedChange(int);
    void gotTetris(QString);

protected:

    //This allows us to update what the player sees.
    void paintEvent(QPaintEvent*) override;

    //This allows the player to input in the game.
    void keyPressEvent(QKeyEvent*) override;

    //This controls the pace of the game.
    void timerEvent(QTimerEvent*) override;

private:

    enum { BoardWidth = 10, BoardHeight = 22 };

    //Returns the TetronimoShape at the (x,y) index, where (0,0) is the
    //bottom left of the board.
    TetronimoShape &shapeAt(int, int);

    //This returns the number of milliseconds between updates of the game.
    int timeoutTime();

    //Deduces the width of each square of a tetronimo, depends on the width
    //of the QFrame.
    int squareWidth();

    //Deduces the height of the each square of a tetronimo, depends of the
    //height of the QFrame.
    int squareHeight();

    //This clears the board by replacing all indeces of board with NoShape.
    void clearBoard();

    //Drops the piece as far as it will go, increasing the player's score.
    void dropDown();

    //Moves the piece down one row when the D key is pressed.
    void oneLineDown();

    //Rewards points for dropping a piece and checks for completed lines.
    void pieceDropped(int);

    //Removes full lines on the game board by searching each row, it also
    //moves rows down when something below it has been removed.
    void removeFullLines();

    //Produces a new piece for the player.
    void newPiece();

    //When the player presses H, the current piece moves to the hold piece
    //and the next piece is moved to current piece.
    void hold();

    //Paints the QLabels with the next and held piece so the player can see
    //them.
    void showNextPiece();
    void showHoldPiece();

    //Attempts the move and returns whether or not it is possible. If it is
    //possible then it returns true and the move occurs.
    bool tryMove(const Tetronimo&, int, int);

    //Draws the blocks on the game board. The blocks are drawn as different
    //colors depending on what TetronimoShape the specific block is.
    void drawSquare(QPainter&, int, int, TetronimoShape);

    //Hold the time, next piece, and hold piece.
    QBasicTimer timer;
    QPointer<QLabel> nextPieceLabel;
    QPointer<QLabel> holdPieceLabel;

    //State which phase the game board is currently in.
    bool Started;
    bool Paused;
    bool Waiting;

    //These are the actual pieces in play, or about to be in play.
    Tetronimo currentPiece;
    Tetronimo nextPiece;
    Tetronimo holdPiece;

    //Holds the position of the (0,0) square of the currently dropping shape.
    int currentX;
    int currentY;

    //Keeps track of lines removed and pieces dropped, these are used to increase
    //the level and thus the speed of the game increasing its difficulty.
    int LinesRemoved;
    int PiecesDropped;

    //Score and level, these are presented to the player by the widget.
    int score;
    int level;

    //An integer multiplier for points and reduces the time between game updates.
    int difficultyMod;

    //The actual board is simply a single array, where each point holds a value
    //corresponding to a TetronimoShape.
    TetronimoShape board[BoardWidth * BoardHeight];
};

#endif
