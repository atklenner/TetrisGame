#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QWidget>
#include <QFrame>

class QLCDNumber;
class QLabel;
class QPushButton;
class GameBoard;

class TetrisGame : public QWidget
{
    Q_OBJECT

public:
    TetrisGame();

private:

    //Function used to make creating labels easier.
    QLabel *createLabel(const QString &text);

    //All the elements in the window except the labels.
    GameBoard *board;
    QLabel *nextPiece;
    QLCDNumber *score;
    QLCDNumber *level;
    QLCDNumber *lines;
    QPushButton *pauseButton;
    QPushButton *quitButton;
    QPushButton *startButton;
};

#endif // TETRISGAME_H
