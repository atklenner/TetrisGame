#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QWidget>
#include <QFrame>

class TetrisGame : public QWidget
{
    Q_OBJECT

public:
    TetrisGame();

private:
    QLabel *createLabel(const QString &text);

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
