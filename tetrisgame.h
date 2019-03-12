#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QMainWindow>

namespace Ui {
class TetrisGame;
}

class TetrisGame : public QMainWindow
{
    Q_OBJECT

public:
    explicit TetrisGame(QWidget *parent = nullptr);
    ~TetrisGame();

private:
    Ui::TetrisGame *ui;
};

#endif // TETRISGAME_H
