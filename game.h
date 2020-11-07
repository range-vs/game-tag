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
    QHBoxLayout* dopLayout;

    CustomLabel* createDice(int r, int c, QString text, bool en = true)
    {
        auto* elem(new CustomLabel(text));
        QObject::connect(elem, SIGNAL(unpressed()), this, SLOT(clickDice()));
        QObject::connect(elem, SIGNAL(pressed()), this, SLOT(colorDice()));

        elem->setCursor(Qt::PointingHandCursor);
        elem->setLineWidth(3);
        elem->setAlignment(Qt::AlignCenter);
        auto font(elem->font());
        font.setPixelSize(40);
        elem->setFont(font);
        QPalette palette;
        palette.setColor(elem->backgroundRole(), Qt::lightGray);
        elem->setPalette(palette);
        elem->setAutoFillBackground(true);
        elem->setEnabled(en);
        gameMap->addWidget(elem, r, c, 1, 1);
        return elem;
    }

    void getGridPosition(QWidget * widget, int* r, int* c)
    {
      auto index(static_cast<int>(gameMap->indexOf(widget)));
      Q_ASSERT(index >= 0);
      auto tmp(-1);
      gameMap->getItemPosition(index, r, c, &tmp, &tmp);
    }

    bool checkFinishGame(const QVector<int>& v)
    {
        for(QVector<int>::value_type i(0); i < v.size() - 1; ++i)
            if(v.at(i) != i + 1)
                return false;
        return true;
    }

    void blockGameMap()
    {
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
            {
                auto* li(gameMap->itemAtPosition(i, j));
                if(!li || !li->widget())
                    continue;
                li->widget()->setEnabled(true);
            }
    }

    bool isCorrectGame(const QVector<int>& v)
    {
        auto inv(0);
        for(QVector<int>::value_type i(0); i < v.size(); ++i)
            if(v.at(i))
                for(auto j(0); j < i; ++j)
                    if(v.at(j) > v.at(i))
                        ++inv;
        for(QVector<int>::value_type i(0); i < v.size(); ++i)
            if(!v.at(i))
            {
                inv += 1 + i / 4;
                return !(inv & 1);
            }
        return true;
    }

    QVector<int> getArrayDice()
    {
        QVector<int> out;
        for(auto i(0); i < 4; ++i)
            for(auto j(0); j < 4; ++j)
            {
                auto* li(gameMap->itemAtPosition(i, j));
                if(!li || !li->widget())
                     out << 0;
                else
                    out << (qobject_cast<CustomLabel*>(li->widget()))->text().toInt();
            }
        return out;
    }

//    void debugOutputGrid()
//    {
//        qDebug() << "DEBUG:";
//        for(int i(0); i < 4; ++i)
//            for(int j(0); j < 4; ++j)
//                if(gameMap->itemAtPosition(i, j))
//                    qDebug() << qobject_cast<CustomLabel*>(gameMap->itemAtPosition(i, j)->widget())->text();
//                else
//                    qDebug() << "nullptr";
//         qDebug() << "DEBUG END";
//    }

public:
    Game(QWidget* obj = nullptr): QWidget(obj), gameMap(nullptr), currentDice(nullptr), mainLabel(nullptr), dopLayout(nullptr){}

    void run(const QApplication& a)
    {
        setWindowTitle("Game tag");
        resize(400,400);
        setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), a.screens().at(0)->availableGeometry()));

        auto mainLayout(new QVBoxLayout());
        mainLayout->setSpacing(30);

        mainLabel = new QLabel("Please, start new game...");
        mainLabel->setAlignment(Qt::AlignCenter);
        mainLabel->setMargin(10);

        gameMap = new QGridLayout();
        gameMap->setSpacing(30);
        for(auto i(0); i < 4; ++i)
        {
            gameMap->setRowStretch(i, 1);
            for(auto j(0); j < 4; ++j)
            {
                gameMap->setColumnStretch(j, 1);
                if((i + 1) * (j + 1) == 16)
                    break;
                createDice(i, j, QString::number(4 * i + (j + 1)), false);
            }
        }

        dopLayout = new QHBoxLayout();
        auto* newGame(new QPushButton("New game"));
        QObject::connect(newGame, SIGNAL(clicked()), this, SLOT(newGame()));
        auto* exit(new QPushButton("Exit"));
        QObject::connect(exit, SIGNAL(clicked()), this, SLOT(close()));
        dopLayout->setSpacing(30);
        dopLayout->addWidget(newGame);
        dopLayout->addWidget(exit);

        mainLayout->addWidget(mainLabel);
        mainLayout->addStretch();
        mainLayout->addLayout(gameMap);
        mainLayout->addStretch();
        mainLayout->addLayout(dopLayout);

        setLayout(mainLayout);

        show();
    }

signals:
    void clicked();
    void resized();

public slots:
    void newGame()
    {
        currentDice = nullptr;
        mainLabel->setText("New game started");

        QVector<int> pos;
        while(true)
        {
            QVector<int> tmpPos{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
            for(auto i(0); i < 16; ++i)
            {
                auto index(0 + rand() % tmpPos.size());
                pos << tmpPos[index];
                tmpPos.remove(index);
            }
            if(isCorrectGame(pos))
                break;
            pos.clear();
        }

        while (gameMap->count())
        {
            auto* child(gameMap->takeAt(0));
            if (child->widget() != nullptr)
                delete (child->widget());
            delete child;
        }

        for(QVector<int>::value_type i(0); i < pos.size(); ++i)
            if(pos[i])
                createDice(i / 4, i % 4, QString::number(pos[i]));
        emit resized();
    }

    void clickDice()
    {
        auto* elem(qobject_cast<CustomLabel*>(sender()));
        if(!elem || !elem->isEnabled())
            return;

        auto palette(elem->palette());
        palette.setColor(elem->foregroundRole(), Qt::black);
        elem->setPalette(palette);

        auto _r(0);
        auto _c(0);
        auto title((QString("Move impossible maked")));
        auto frame(QFrame::Box | QFrame::Raised);
        getGridPosition(elem, &_r, &_c);
        QVector<QPair<int, int>> coords{{_r, _c - 1}, {_r - 1, _c}, {_r, _c + 1}, {_r + 1, _c}};
        for(auto&& c: coords)
        {
            if((c.first >= 0 && c.first < 4) && (c.second >= 0 && c.second < 4))
            {
                auto* wi(gameMap->itemAtPosition(c.first, c.second));
                if(!wi)
                {
                    auto* newElem(elem->clone());
                    QObject::connect(newElem, SIGNAL(unpressed()), this, SLOT(clickDice()));
                    QObject::connect(newElem, SIGNAL(pressed()), this, SLOT(colorDice()));
                    gameMap->addWidget(newElem, c.first, c.second);
                    gameMap->removeWidget(elem);
                    elem->deleteLater();

                    auto map(getArrayDice());
                    if(checkFinishGame(map))
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

    void colorDice()
    {
        auto* elem(qobject_cast<CustomLabel*>(sender()));
        if(!elem || !elem->isEnabled())
            return;
        if(currentDice)
            currentDice->setFrameStyle(QFrame::NoFrame);
        elem->setFrameStyle(QFrame::Box | QFrame::Plain);
        auto palette(elem->palette());
        palette.setColor(elem->foregroundRole(), Qt::red);
        elem->setPalette(palette);
    }
};
