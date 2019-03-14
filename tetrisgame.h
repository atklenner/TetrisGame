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
    GameBoard *gameBoard;
    QLabel *nextPiece;
    QLabel *holdPiece;
    QLCDNumber *score;
    QLCDNumber *level;
    QLCDNumber *lines;
    QPushButton *pause;
    QPushButton *start;

};

#endif // TETRISGAME_H
