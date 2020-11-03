#pragma once

#include "customlabel.h"

#include <QtGui>
#include <QtWidgets>
#include <QVector>

class Game: public QWidget
{
    Q_OBJECT

    QGridLayout* gameMap;
    CustomLabel* currentDice;
    QLabel* mainLabel;

    CustomLabel* createDice(int r, int c, QString text, bool en = true)
    {
        CustomLabel* elem = new CustomLabel(text);
        //QObject::connect(elem, SIGNAL(clicked()), this, SLOT(clickDice()));
        elem->setCursor(Qt::PointingHandCursor);
        elem->setLineWidth(3);
        elem->resize(50, 50);
        elem->setAlignment(Qt::AlignCenter);
        QFont font = elem->font();
        font.setPixelSize(20);
        elem->setFont(font);
        QPalette palette;
        palette.setColor(elem->backgroundRole(), Qt::blue);
        elem->setPalette(palette);
        elem->setAutoFillBackground(true);
        elem->setEnabled(en);
        gameMap->addWidget(elem, r, c);
        return elem;
    }

    void getGridPosition(QWidget * widget, int* r, int* c)
    {
      int index = gameMap->indexOf(widget);
      Q_ASSERT(index >= 0);
      int tmp(-1);
      gameMap->getItemPosition(index, r, c, &tmp, &tmp);
    }

    bool checkFinishGame()
    {
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
            {
                if((i + 1) * (j + 1) == 16)
                    return true;
                QLayoutItem* li = gameMap->itemAtPosition(i, j);
                if(!li || !li->widget())
                    return false;
                if(qobject_cast<CustomLabel*>(li->widget())->text() != QString::number(i * 4 + j + 1))
                    return false;
            }
        return true;
    }

    void blockGameMap()
    {
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
            {
                if((i + 1) * (j + 1) == 16)
                    return;
                QLayoutItem* li = gameMap->itemAtPosition(i, j);
                if(!li || !li->widget())
                    return;
                li->widget()->setEnabled(false);
            }
    }

    void debugOutputGrid()
    {
        qDebug() << "DEBUG:";
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
                if(gameMap->itemAtPosition(i, j))
                    qDebug() << qobject_cast<CustomLabel*>(gameMap->itemAtPosition(i, j)->widget())->text();
                else
                    qDebug() << "nullptr";
         qDebug() << "DEBUG END";
    }

public:
    Game(QWidget* obj = nullptr): QWidget(obj), currentDice(nullptr){}

    void run(const QApplication& a)
    {
        setWindowTitle("Game tag");
        resize(400, 450);
        setGeometry(QStyle::alignedRect(
                                 Qt::LeftToRight,
                                 Qt::AlignCenter,
                                 size(),
                                 a.screens().at(0)->availableGeometry()
                             ));

        setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->setSpacing(15);

        mainLabel = new QLabel("Please, start new game...");
        //mainLabel->setContentsMargins(10,0,10,0);
        mainLabel->setAlignment(Qt::AlignCenter);
        mainLabel->setMaximumSize(QSize(400, 30));

        gameMap = new QGridLayout();
        gameMap->setSpacing(10);
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
            {
                if((i + 1) * (j + 1) == 16)
                    break;
                CustomLabel* elem = createDice(i, j, QString::number(4 * i + (j + 1)), false);
                QObject::connect(elem, SIGNAL(clicked()), this, SLOT(clickDice()));
            }

        QHBoxLayout* dopLayout = new QHBoxLayout();
        QPushButton* newGame = new QPushButton("New game");
        QObject::connect(newGame, SIGNAL(clicked()), this, SLOT(newGame()));
        QPushButton* exit = new QPushButton("Exit");
        QObject::connect(exit, SIGNAL(clicked()), this, SLOT(close()));
        dopLayout->setSpacing(30);
        dopLayout->addWidget(newGame);
        dopLayout->addWidget(exit);

        mainLayout->addWidget(mainLabel);
        mainLayout->addLayout(gameMap);
        mainLayout->addLayout(dopLayout);

        setLayout(mainLayout);

        show();
    }

signals:
    void clicked();

public slots:
    void newGame()
    {
        currentDice = nullptr;
        mainLabel->setText("New game started");

        QVector<int> pos = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
        QVector<int> posGrid = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


        while (gameMap->count())
        {
            QLayoutItem* child = gameMap->takeAt(0);
            if (child->widget() != nullptr)
                delete (child->widget());
            delete child;
        }

        int s = pos.size();
        for(int i (0); i < s; ++i)
        {
            int indexPos = 0 + rand() % pos.size();
            int indexGrid = 0 + rand() % posGrid.size();
            CustomLabel* elem = createDice(posGrid[indexGrid] / 4, posGrid[indexGrid] % 4, QString::number(pos[indexPos] + 1));
            QObject::connect(elem, SIGNAL(clicked()), this, SLOT(clickDice()));
            pos.remove(indexPos);
            posGrid.remove(indexGrid);
        }

    }

    void clickDice()
    {
        CustomLabel* elem = qobject_cast<CustomLabel*>(sender());
        if(!elem || !elem->isEnabled())
            return;

        int _r(0);
        int _c(0);
        QString title = "Move impossible maked";
        int frame = QFrame::Box | QFrame::Raised;
        getGridPosition(elem, &_r, &_c);
        QVector<QPair<int, int>> coords = {{_r, _c - 1}, {_r - 1, _c}, {_r, _c + 1}, {_r + 1, _c}};
        for(auto&& c: coords)
        {
            if((c.first >= 0 && c.first < 4) && (c.second >= 0 && c.second < 4))
            {
                QLayoutItem* wi = gameMap->itemAtPosition(c.first, c.second);
                if(!wi)
                {
                    CustomLabel* newElem = elem->clone();
                    QObject::connect(newElem, SIGNAL(clicked()), this, SLOT(clickDice()));
                    gameMap->addWidget(newElem, c.first, c.second);
                    gameMap->removeWidget(elem);
                    elem->deleteLater();

                    if(checkFinishGame())
                    {
                        title = "Game over! You win";
                        frame = QFrame::NoFrame;
                        blockGameMap();
                        break;
                    }

                    title = "Move maked";
                    frame = QFrame::Box | QFrame::Plain;
                    elem = newElem;
                    break;
                }
                else if(qobject_cast<QWidget*>(wi->widget()))
                    continue;
            }
        }
        mainLabel->setText(title);
        elem->setFrameStyle(frame);
        if(elem != currentDice && currentDice)
            currentDice->setFrameStyle(QFrame::NoFrame);
        currentDice = elem;
    }

};
