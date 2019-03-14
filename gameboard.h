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

    void setNextPieceLabel(QLabel *label);
    void setHoldPieceLabel(QLabel *label);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void start();
    void pause();

signals:
    void scoreChanged(int score);
    void levelChanged(int level);
    void linesRemovedChanged(int numLines);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    enum { BoardWidth = 10, BoardHeight = 22 };

    TetronimoShape &shapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }
    int timeoutTime() { return 1000 / (1 + level); }
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
    bool isStarted;
    bool isPaused;
    bool isWaitingAfterLine;
    Tetronimo curPiece;
    Tetronimo nextPiece;
    Tetronimo holdPiece;
    int curX;
    int curY;
    int numLinesRemoved;
    int numPiecesDropped;
    int score;
    int level;
    TetronimoShape board[BoardWidth * BoardHeight];
};
//! [1]

#endif // GAMEBOARD_H
