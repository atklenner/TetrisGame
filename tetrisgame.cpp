#include <QtWidgets>
#include "tetrisgame.h"
#include "gameboard.h"
#include "ui_tetrisgame.h"

//Constructor for the whole widget
TetrisGame::TetrisGame() :
    ui(new Ui::Form) {

    ui->setupUi(this);

    ui->gameBoard->setNextPieceLabel(ui->nextPiece);
    ui->gameBoard->setHoldPieceLabel(ui->holdPiece);


    //Connecting the buttons to the functions in the gameboard class.
    connect(ui->start, &QPushButton::clicked, ui->gameBoard, &GameBoard::start);
    connect(ui->pause, &QPushButton::clicked, ui->gameBoard, &GameBoard::pause);
    connect(ui->difficulty, &QSlider::valueChanged, ui->gameBoard, &GameBoard::difficulty);

    //Connecting the output of the gameboard to the displays.
    connect(ui->gameBoard, &GameBoard::scoreChange,
            ui->score, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->gameBoard, &GameBoard::levelChange,
            ui->level, QOverload<int>::of(&QLCDNumber::display));
    connect(ui->gameBoard, &GameBoard::linesRemovedChange,
            ui->lines, QOverload<int>::of(&QLCDNumber::display));
//    connect(gameBoard, &GameBoard::gotTetris,
//            ui->tetris, &QLabel::setText);

        setWindowTitle(tr("Tetris"));
}

TetrisGame::~TetrisGame() {
    delete ui;
}
