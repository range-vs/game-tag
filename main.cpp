#include "game.h"

#include <QtGui>
#include <QtWidgets>

#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(nullptr));

    Game* game = new Game();
    game->run(a);
    int result = a.exec();
    delete game;

    return result;
}
