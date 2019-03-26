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
    void scoreChange(int);
    void levelChange(int);
    void linesRemovedChange(int);
    void gotTetris(QString);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    enum { BoardWidth = 10, BoardHeight = 22 };

    TetronimoShape &shapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }
    int timeoutTime() { return 1000 / (1 + level + difficultyMod); }
    int squareWidth() { return contentsRect().width() / BoardWidth; }
    int squareHeight() { return contentsRect().height() / BoardHeight; }
    void clearBoard();
    void dropDown();
    void oneLineDown();
    void pieceDropped(int dropHeight);
    void removeFullLines();
    void newPiece();
    void hold();
    void showNextPiece();
    void showHoldPiece();
    bool tryMove(const Tetronimo &newPiece, int newX, int newY);
    void drawSquare(QPainter &painter, int x, int y, TetronimoShape shape);

    QBasicTimer timer;
    QPointer<QLabel> nextPieceLabel;
    QPointer<QLabel> holdPieceLabel;
    bool Started;
    bool Paused;
    bool Waiting;
    Tetronimo currentPiece;
    Tetronimo nextPiece;
    Tetronimo holdPiece;
    int currentX;
    int currentY;
    int numLinesRemoved;
    int numPiecesDropped;
    int score;
    int level;
    int difficultyMod;
    TetronimoShape board[BoardWidth * BoardHeight];
};

#endif
