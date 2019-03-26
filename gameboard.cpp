#include "gameboard.h"
#include <QtWidgets>

GameBoard::GameBoard(QWidget* parent) : QFrame(parent) {
    //The only reason the frame style is set up here is because I couldn't
    //find it in the designer.
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    Started = false;
    Paused = false;
    clearBoard();
    nextPiece.setRandomShape();
}

void GameBoard::setNextPieceLabel(QLabel* label) {
    nextPieceLabel = label;
}

void GameBoard::setHoldPieceLabel(QLabel* label) {
    holdPieceLabel = label;
}

void GameBoard::start() {
    if (Paused) {
        return;
    }
    Started = true;
    Waiting = false;
    LinesRemoved = 0;
    PiecesDropped = 0;
    score = 0;
    level = 1;
    difficultyMod = 0;
    clearBoard();
    emit linesRemovedChange(LinesRemoved);
    emit scoreChange(score);
    emit levelChange(level);
    newPiece();
    timer.start(timeoutTime(), this);
}

void GameBoard::pause() {
    if (!Started) {
        return;
    }

    Paused = !Paused;
    if (Paused) {
        timer.stop();
    } else {
        timer.start(timeoutTime(), this);
    }
    update();
}

void GameBoard::difficulty(int diff) {
    difficultyMod = diff;
    if (Started) {
        timer.start(timeoutTime(), this);
    }
}

//This presents the player with the directions on the board and when paused.
//It also scans every element in the board array and draws what type of
//square is contained in it.
void GameBoard::paintEvent(QPaintEvent* event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    QRect rect = contentsRect();
    //Tells the player how to resume the game.
    if (Paused) {
        painter.drawText(rect, Qt::AlignCenter, tr("Paused\nPress P to resume."));
        return;
    }
    //Adding the directions, they will only show up before you hit the start button.
    if(currentPiece.shape() == NoShape && !Started) {
        painter.drawText(rect, Qt::AlignCenter,
                         tr("Controls\n\nLeft & Right: move piece\n"
                            "Up & Down: rotate piece\nSpace Bar: drop"
                            " piece\nD: drop piece one line\nP: pause"
                            " game\nH: hold current piece"));
    }
    int boardTop = rect.bottom() - BoardHeight*squareHeight();
    //This is where the drawing of squares happens.
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            TetronimoShape shape = shapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape) {
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
            }
        }
    }
    //This draws the current falling piece.
    if (currentPiece.shape() != NoShape) {
        for (int i = 0; i < 4; ++i) {
            int x = currentX + currentPiece.x(i);
            int y = currentY - currentPiece.y(i);
            drawSquare(painter, rect.left() + x * squareWidth(),
                       boardTop + (BoardHeight - y - 1) * squareHeight(),
                       currentPiece.shape());
        }
    }
}

void GameBoard::keyPressEvent(QKeyEvent* event) {
    if (!Started || Paused || currentPiece.shape() == NoShape) {
        //This lets the player resume the game by pressing the P key when the
        //game is paused.
        if(Paused) {
            switch(event->key()) {
            case Qt::Key_P:
                pause();
                break;
            default:
                QFrame::keyPressEvent(event);
                return;
            }
        }
        QFrame::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
    case Qt::Key_Left:
        tryMove(currentPiece, currentX - 1, currentY);
        break;
    case Qt::Key_Right:
        tryMove(currentPiece, currentX + 1, currentY);
        break;
    case Qt::Key_Down:
        tryMove(currentPiece.rotateRight(), currentX, currentY);
        break;
    case Qt::Key_Up:
        tryMove(currentPiece.rotateLeft(), currentX, currentY);
        break;
    case Qt::Key_Space:
        dropDown();
        break;
    case Qt::Key_D:
        oneLineDown();
        break;
    case Qt::Key_H:
        hold();
        break;
    case Qt::Key_P:
        pause();
        break;
    default:
        QFrame::keyPressEvent(event);
    }
}

void GameBoard::timerEvent(QTimerEvent* event) {
    if (event->timerId() == timer.timerId()) {
        if (Waiting) {
            Waiting = false;
            newPiece();
            timer.start(timeoutTime(), this);
        } else {
            oneLineDown();
        }
    } else {
        QFrame::timerEvent(event);
    }
}

//This returns the type of square at a given (x,y) where (0,0) is the bottom
//left of the board.
TetronimoShape &GameBoard::shapeAt(int x, int y) {
    return board[(y * BoardWidth) + x];
}

//This helps reinitialize the timer, as you can see when the level or
//difficulty increase the number of milliseconds between updates
//decreases.
int GameBoard::timeoutTime() {
    return 1000 / (1 + level + difficultyMod);
}

int GameBoard::squareWidth() {
    return contentsRect().width() / BoardWidth;
}

int GameBoard::squareHeight() {
    return contentsRect().height() / BoardHeight;
}

//Clears the board by filling each element of the array with a NoShape tetronimo.
void GameBoard::clearBoard() {
    for (int i = 0; i < BoardHeight * BoardWidth; ++i) {
        board[i] = NoShape;
    }
}

//This tests each move of the piece one row down, once it cannot move any more
//the score is increased relative to the rows dropped.
void GameBoard::dropDown() {
    int dropHeight = 0;
    int newY = currentY;
    while (newY > 0) {
        if (!tryMove(currentPiece, currentX, newY - 1)) {
            break;
        }
        --newY;
        ++dropHeight;
    }
    pieceDropped(dropHeight);
}

void GameBoard::oneLineDown() {
    if (!tryMove(currentPiece, currentX, currentY - 1)) {
        pieceDropped(0);
    }
}

void GameBoard::pieceDropped(int dropHeight) {
    for (int i = 0; i < 4; ++i) {
        int x = currentX + currentPiece.x(i);
        int y = currentY - currentPiece.y(i);
        shapeAt(x, y) = currentPiece.shape();
    }
    ++PiecesDropped;
    if (PiecesDropped % 25 == 0) {
        ++level;
        timer.start(timeoutTime(), this);
        emit levelChange(level);
    }
    score += (dropHeight + 7) * (difficultyMod + 1);
    emit scoreChange(score);
    removeFullLines();
    if (!Waiting) {
        newPiece();
    }
}

//Scans the board for full lines.
void GameBoard::removeFullLines() {
    int numFullLines = 0;
    //Starting from the top of the board, this checks each square, if it is
    //empty then the line is not full and it skips to the next. This starts
    //from the top as to avoid missing a full line.
    for (int i = BoardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;
        for (int j = 0; j < BoardWidth; ++j) {
            if (shapeAt(j, i) == NoShape) {
                lineIsFull = false;
                break;
            }
        }
        //If the line is full then replace it with the one above it.
        if (lineIsFull) {
            ++numFullLines;
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j) {
                    shapeAt(j, k) = shapeAt(j, k + 1);
                }
            }
            for (int j = 0; j < BoardWidth; ++j) {
                shapeAt(j, BoardHeight - 1) = NoShape;
            }
        }

    }
    //Awards the player points for filling lines.
    if (numFullLines > 0) {
        LinesRemoved += numFullLines;
        score += 10 * numFullLines * (difficultyMod + 1);
        emit linesRemovedChange(LinesRemoved);
        emit scoreChange(score);
        //This tells the player they got a tetris.
        if (numFullLines == 4) {
            emit gotTetris("TETRIS!");
        }
        timer.start(500, this);
        Waiting = true;
        currentPiece.setShape(NoShape);
        update();
    }
}

//Creates a new piece and checks that it can move, if it cannot move then
//the game ends.
void GameBoard::newPiece() {
    currentPiece = nextPiece;
    nextPiece.setRandomShape();
    showNextPiece();
    currentX = BoardWidth / 2 + 1;
    currentY = BoardHeight - 1 + currentPiece.minY();
    if (!tryMove(currentPiece, currentX, currentY)) {
        currentPiece.setShape(NoShape);
        timer.stop();
        Started = false;
    }
}

//This either stores a piece into an empty container or swaps it with a piece
//that has been previously stored.
void GameBoard::hold() {
    if (holdPiece.shape() == NoShape ) {
        holdPiece = currentPiece;
        newPiece();
        showHoldPiece();
    } else {
        Tetronimo dummy = currentPiece;
        currentPiece = holdPiece;
        holdPiece = dummy;
        showHoldPiece();
    }
}

//These next two simply draw the piece for the QLabel in the ui.
void GameBoard::showNextPiece() {
    if (!nextPieceLabel) {
        return;
    }
    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
    int dy = nextPiece.maxY() - nextPiece.minY() + 1;
    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), nextPieceLabel->palette().background());
    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.x(i) - nextPiece.minX();
        int y = nextPiece.y(i) - nextPiece.minY();
        drawSquare(painter, x * squareWidth(), y * squareHeight(),
                   nextPiece.shape());
    }
    nextPieceLabel->setPixmap(pixmap);
}

void GameBoard::showHoldPiece() {
    if (!holdPieceLabel) {
        return;
    }
    int dx = holdPiece.maxX() - holdPiece.minX() + 1;
    int dy = holdPiece.maxY() - holdPiece.minY() + 1;
    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), holdPieceLabel->palette().background());
    for (int i = 0; i < 4; ++i) {
        int x = holdPiece.x(i) - holdPiece.minX();
        int y = holdPiece.y(i) - holdPiece.minY();
        drawSquare(painter, x * squareWidth(), y * squareHeight(),
                   holdPiece.shape());
    }
    holdPieceLabel->setPixmap(pixmap);
}

//This first calculates where the piece would move if it were to move, then
//if the move is valid (i.e. not moving off the board, into another piece),
//then the move is made official and presented to the player.
bool GameBoard::tryMove(const Tetronimo &newPiece, int newX, int newY) {
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight) {
            return false;
        }
        if (shapeAt(x, y) != NoShape) {
            return false;
        }
    }
    currentPiece = newPiece;
    currentX = newX;
    currentY = newY;
    update();
    return true;
}

//This checks each square on the board and paints it a specific color depending
//on the shape that is stored there.
void GameBoard::drawSquare(QPainter &painter, int x,
                           int y, TetronimoShape shape) {
    static const QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };
    QColor color = colorTable[int(shape)];
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                     color);
    painter.setPen(color.light());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);
    painter.setPen(color.dark());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}
