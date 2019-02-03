#include <Qt/PlayerDemo.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PlayerDemo demo;
    demo.show();
    return app.exec();
}