#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QWidget>
#include <QFrame>

class QLCDNumber;
class QLabel;
class QPushButton;
class QSlider;
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
    GameBoard *gameBoard;
    QLabel *nextPiece;
    QLabel *holdPiece;
    QLabel *tetris;
    QLCDNumber *score;
    QLCDNumber *level;
    QLCDNumber *lines;
    QPushButton *pause;
    QPushButton *start;
    QSlider *difficulty;

};

#endif // TETRISGAME_H
