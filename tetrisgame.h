#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QWidget>
#include <QFrame>

class QLCDNumber;
class QLabel;
class QPushButton;
class QSlider;
class GameBoard;

namespace Ui {
class Form;
}

class TetrisGame : public QWidget
{
    Q_OBJECT

public:
    TetrisGame();
    ~TetrisGame();

private:
    Ui::Form *ui;

};

#endif // TETRISGAME_H
