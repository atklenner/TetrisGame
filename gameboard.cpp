#include "gameboard.h"
#include <QtWidgets>

GameBoard::GameBoard(QWidget *parent) : QFrame(parent) {
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
    Started = false;
    Paused = false;
    clearBoard();

    nextPiece.setRandomShape();
}

void GameBoard::setNextPieceLabel(QLabel *label) {
    nextPieceLabel = label;
}

void GameBoard::setHoldPieceLabel(QLabel *label) {
    holdPieceLabel = label;
}

QSize GameBoard::sizeHint() const {
    return QSize(BoardWidth * 15 + frameWidth() * 2,
                 BoardHeight * 15 + frameWidth() * 2);
}

QSize GameBoard::minimumSizeHint() const {
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}

void GameBoard::start() {
    if (Paused)
        return;

    Started = true;
    Waiting = false;
    numLinesRemoved = 0;
    numPiecesDropped = 0;
    score = 0;
    level = 1;
    difficultyMod = 0;
    clearBoard();

    emit linesRemovedChange(numLinesRemoved);
    emit scoreChange(score);
    emit levelChange(level);

    newPiece();
    timer.start(timeoutTime(), this);
}

void GameBoard::pause() {
    if (!Started)
        return;

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
    timer.start(timeoutTime(), this);
}

void GameBoard::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();

    if (Paused) {
        painter.drawText(rect, Qt::AlignCenter, tr("Paused\nPress P to resume."));
        return;
    }

    //Adding the directions, they will only show up before you hit the start button.
    if(currentPiece.shape() == NoShape && !Started) {
        painter.drawText(rect, Qt::AlignCenter, tr("Controls\n\nLeft & Right: move piece\n"
                                                   "Up & Down: rotate piece\nSpace Bar: drop"
                                                   " piece\nD: drop piece one line\nP: pause"
                                                   " game\nH: hold current piece"));
    }

    int boardTop = rect.bottom() - BoardHeight*squareHeight();

    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            TetronimoShape shape = shapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape)
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
        }
    }
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

void GameBoard::keyPressEvent(QKeyEvent *event) {
    if (!Started || Paused || currentPiece.shape() == NoShape) {
        //I added this. If you don't ask if the game is paused then you could
        //pause the game before it has actually started, this results in
        //undefined behavior.
        if(Paused) {
            switch(event->key()) {
            case Qt::Key_P:
                pause();
                break;
            default:
                QFrame::keyPressEvent(event);
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
    //I added this so you can pause while playing by pressing the p key.
    case Qt::Key_P:
        pause();
        break;
    default:
        QFrame::keyPressEvent(event);
    }
}

void GameBoard::timerEvent(QTimerEvent *event) {
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

void GameBoard::clearBoard() {
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
        board[i] = NoShape;
}

void GameBoard::dropDown() {
    int dropHeight = 0;
    int newY = currentY;
    while (newY > 0) {
        if (!tryMove(currentPiece, currentX, newY - 1))
            break;
        --newY;
        ++dropHeight;
    }
    pieceDropped(dropHeight);
}

void GameBoard::oneLineDown() {
    if (!tryMove(currentPiece, currentX, currentY - 1))
        pieceDropped(0);
}

void GameBoard::pieceDropped(int dropHeight) {
    for (int i = 0; i < 4; ++i) {
        int x = currentX + currentPiece.x(i);
        int y = currentY - currentPiece.y(i);
        shapeAt(x, y) = currentPiece.shape();
    }

    ++numPiecesDropped;
    if (numPiecesDropped % 25 == 0) {
        ++level;
        timer.start(timeoutTime(), this);
        emit levelChange(level);
    }

    score += (dropHeight + 7) * (difficultyMod + 1);
    emit scoreChange(score);
    removeFullLines();

    if (!Waiting)
        newPiece();
}

void GameBoard::removeFullLines() {
    int numFullLines = 0;

    for (int i = BoardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < BoardWidth; ++j) {
            if (shapeAt(j, i) == NoShape) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            ++numFullLines;
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j)
                    shapeAt(j, k) = shapeAt(j, k + 1);
            }
            for (int j = 0; j < BoardWidth; ++j)
                shapeAt(j, BoardHeight - 1) = NoShape;
        }

    }

    if (numFullLines > 0) {
        numLinesRemoved += numFullLines;
        score += 10 * numFullLines * (difficultyMod + 1);
        emit linesRemovedChange(numLinesRemoved);
        emit scoreChange(score);
        if (numFullLines == 4) {
            emit gotTetris("Tetris!");
        }
        timer.start(500, this);
        emit gotTetris("");
        Waiting = true;
        currentPiece.setShape(NoShape);
        update();
    }

}

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

void GameBoard::showNextPiece() {
    if (!nextPieceLabel)
        return;

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
    if (!holdPieceLabel)
        return;

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

bool GameBoard::tryMove(const Tetronimo &newPiece, int newX, int newY) {
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (shapeAt(x, y) != NoShape)
            return false;
    }

    currentPiece = newPiece;
    currentX = newX;
    currentY = newY;
    update();
    return true;
}

void GameBoard::drawSquare(QPainter &painter, int x, int y, TetronimoShape shape) {
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
