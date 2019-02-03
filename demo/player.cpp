#include "Qt/PlayerWidget.h"
#include <Qt/AudioPlaybackThread.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	openshot::AudioDeviceManagerSingleton::Instance(2);
    PlayerWidget demo;
    demo.show();
    auto ret = app.exec();

	openshot::AudioDeviceManagerSingleton::Finialize();
	return ret;
}