#include "centralsquarelayout.h"
#include "game.h"
#include "testlbl.h"

#include <QtGui>
#include <QtWidgets>

#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(nullptr));
    auto* game = new Game();
    game->run(a);
    auto result(a.exec());
    delete game;
    return result;

//    QApplication a(argc, argv);
//    QWidget widget;
//    QVBoxLayout l;
//    l.addWidget(new QLabel("test"));
//    l.addStretch();
//    QGridLayout layout;
//    for( int i =0 ; i < 4; i++ )
//    {
//        layout.setRowStretch( i, 1);
//        for ( int j = 0; j < 4; j++)
//        {
//            layout.setColumnStretch( j, 1 );
//            TestLbl *temp = new TestLbl( "1" );
//            temp->setFrameStyle(QFrame::Box | QFrame::Plain);
//            layout.addWidget( temp, i, j, 1, 1 );
//        }
//    }
//    l.addLayout(&layout);
//    l.addStretch();
//    QHBoxLayout bl;
//    bl.addWidget(new QLabel("test"));
//    bl.addStretch();
//    bl.addWidget(new QLabel("test"));
//    l.addLayout(&bl);
//    widget.setLayout( &l );
//    widget.show();
//    a.exec();
//    return 0;

//    QApplication a(argc, argv);
//    QWidget w;
//    QVBoxLayout l;
//    l.addWidget(new QLabel("test"));
//    l.addStretch(1);
//    CentralSquareLayout square;
//    QGridLayout layout;
//    for( int i =0 ; i < 4; i++ )
//    {
//        for ( int j = 0; j < 4; j++)
//        {
//            TestLbl *temp = new TestLbl( "1" );
//            temp->setFrameStyle(QFrame::Box | QFrame::Plain);
//            layout.addWidget( temp, i, j );
//        }
//    }
//    square.addItem(&layout);
//    l.addLayout(&square, 1); // второй параметр - насколько нужно растягивать центральный квадрат в сравнении с растягиванием пробелов сверху и снизу
//    l.addStretch(1);
//    w.setLayout(&l);
//    w.show();
//    a.exec();
//    return 0;
}
