#include <QtWidgets>
#include "tetrisgame.h"
#include "gameboard.h"

//Constructor for the whole widget
TetrisGame::TetrisGame() {

    //Creating gameboard object, this contains the actual play area.
    gameBoard = new GameBoard;

    //Creating the label which tells the player the next piece.
    nextPiece = new QLabel;
    nextPiece->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPiece->setAlignment(Qt::AlignCenter);
    gameBoard->setNextPieceLabel(nextPiece);

    //Creating the label which tells the player what piece is being held.
    holdPiece = new QLabel;
    holdPiece->setFrameStyle(QFrame::Box | QFrame::Raised);
    holdPiece->setAlignment(Qt::AlignCenter);
    gameBoard->setHoldPieceLabel(holdPiece);

//    tetris = new QLabel;

    //Reports the score to the player.
    score = new QLCDNumber(5);
    score->setSegmentStyle(QLCDNumber::Filled);
    level = new QLCDNumber(2);
    level->setSegmentStyle(QLCDNumber::Filled);
    lines = new QLCDNumber(5);
    lines->setSegmentStyle(QLCDNumber::Filled);

    //Creating difficulty slider.
    difficulty = new QSlider(Qt::Horizontal);
    difficulty->setFocusPolicy(Qt::NoFocus);
    difficulty->setMaximum(9);
    difficulty->setMinimum(0);

    //Buttons to start and pause the game.
    start = new QPushButton(tr("&Start"));
    start->setFocusPolicy(Qt::NoFocus);
    pause = new QPushButton(tr("&Pause"));
    pause->setFocusPolicy(Qt::NoFocus);

    //Connecting the buttons to the functions in the gameboard class.
    connect(start, &QPushButton::clicked, gameBoard, &GameBoard::start);
    connect(pause, &QPushButton::clicked, gameBoard, &GameBoard::pause);
    connect(difficulty, &QSlider::valueChanged, gameBoard, &GameBoard::difficulty);

    //Connecting the output of the gameboard to the displays.
    connect(gameBoard, &GameBoard::scoreChange,
            score, QOverload<int>::of(&QLCDNumber::display));
    connect(gameBoard, &GameBoard::levelChange,
            level, QOverload<int>::of(&QLCDNumber::display));
    connect(gameBoard, &GameBoard::linesRemovedChange,
            lines, QOverload<int>::of(&QLCDNumber::display));
//    connect(gameBoard, &GameBoard::gotTetris,
//            tetris, &QLabel::setText);

    //Setting up the layout of the widget, by grid.
    QGridLayout *layout = new QGridLayout;
        layout->addWidget(createLabel(tr("NEXT")), 0, 0);
        layout->addWidget(nextPiece, 1, 0);
        layout->addWidget(createLabel(tr("HOLD")));
        layout->addWidget(holdPiece, 3, 0);
        layout->addWidget(createLabel(tr("LEVEL")), 4, 0);
        layout->addWidget(level, 5, 0);
        layout->addWidget(start, 4, 2);
        layout->addWidget(gameBoard, 0, 1, 6, 1);
        layout->addWidget(createLabel(tr("SCORE")), 0, 2);
        layout->addWidget(score, 1, 2);
        layout->addWidget(createLabel(tr("LINES REMOVED")), 2, 2);
        layout->addWidget(lines, 3, 2);
        layout->addWidget(pause, 5, 2);
        layout->addWidget(createLabel(tr("DIFFICULTY")), 6, 1);
        layout->addWidget(difficulty, 7, 0, 7, 3);
        setLayout(layout);

        setWindowTitle(tr("Tetris"));
        resize(550, 370);
}

//This function makes writing the individual widgets easier.
QLabel *TetrisGame::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return label;
}
