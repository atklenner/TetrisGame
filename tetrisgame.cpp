#include <QtWidgets>
#include "tetrisgame.h"
#include "gameboard.h"
#include "ui_tetrisgame.h"

//Constructor for the whole widget
TetrisGame::TetrisGame() :
    ui(new Ui::Form) {

    //These all set up the ui and the signals and slots between the GameBoard
    //and TetrisGame.
    ui->setupUi(this);

    ui->gameBoard->setNextPieceLabel(ui->nextPiece);
    ui->gameBoard->setHoldPieceLabel(ui->holdPiece);

    connect(ui->start, &QPushButton::clicked, ui->gameBoard, &GameBoard::start);
    connect(ui->pause, &QPushButton::clicked, ui->gameBoard, &GameBoard::pause);
    connect(ui->difficulty, &QSlider::valueChanged, ui->gameBoard, &GameBoard::difficulty);

    connect(ui->gameBoard, &GameBoard::scoreChange,
            ui->score, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->gameBoard, &GameBoard::levelChange,
            ui->level, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->gameBoard, &GameBoard::linesRemovedChange,
            ui->lines, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->gameBoard, &GameBoard::gotTetris,
            ui->tetris, &QLabel::setText);

    setWindowTitle(tr("Tetris"));
}

TetrisGame::~TetrisGame() {
    delete ui;
}
