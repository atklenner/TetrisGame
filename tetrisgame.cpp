#include "tetrisgame.h"
#include "ui_tetrisgame.h"

TetrisGame::TetrisGame() {
    gameBoard = new GameBoard;

    score = new QLCDNumber(5);
    score->setSegmentStyle(QLCDNumber::Filled);

    startButton = new QPushButton(tr("&Start"));
    startButton->setFocusPolicy(Qt::NoFocus);

    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setFocusPolicy(Qt::NoFocus);

    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setFocusPolicy(Qt::NoFocus);

    connect(startButton, &QPushButton::clicked, board, &GameBoard::start);
    connect(quitButton , &QPushButton::clicked, qApp, &QApplication::quit);
    connect(pauseButton, &QPushButton::clicked, board, &GameBoard::pause);

    connect(board, &GameBoard::scoreChanged,
            score, QOverload<int>::of(&QLCDNumber::display));

    connect(board, &GameBoard::levelChanged,
            level, QOverload<int>::of(&QLCDNumber::display));

    connect(board, &GameBoard::linesRemovedChanged,
            lines, QOverload<int>::of(&QLCDNumber::display));

    QGridLayout *layout = new QGridLayout;
        layout->addWidget(createLabel(tr("NEXT")), 0, 0);
        layout->addWidget(nextPieceLabel, 1, 0);
        layout->addWidget(createLabel(tr("LEVEL")), 2, 0);
        layout->addWidget(levelLcd, 3, 0);
        layout->addWidget(startButton, 4, 0);
        layout->addWidget(board, 0, 1, 6, 1);
        layout->addWidget(createLabel(tr("SCORE")), 0, 2);
        layout->addWidget(scoreLcd, 1, 2);
        layout->addWidget(createLabel(tr("LINES REMOVED")), 2, 2);
        layout->addWidget(linesLcd, 3, 2);
        layout->addWidget(quitButton, 4, 2);
        layout->addWidget(pauseButton, 5, 2);
        setLayout(layout);

        setWindowTitle(tr("Tetrix"));
        resize(550, 370);
}

QLabel *TetrixWindow::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return label;
}
