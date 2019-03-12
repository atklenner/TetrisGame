#include "tetrisgame.h"
#include "ui_tetrisgame.h"

TetrisGame::TetrisGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TetrisGame)
{
    ui->setupUi(this);
}

TetrisGame::~TetrisGame()
{
    delete ui;
}
