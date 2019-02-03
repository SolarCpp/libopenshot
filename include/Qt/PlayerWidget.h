

#ifndef OPENSHOT_PLAYER_DEMO_H
#define OPENSHOT_PLAYER_DEMO_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtGui/qevent.h>
#include <QTimer>
#include <QApplication>
#include "PlayerControls.h"

// Define the QtPlayer without including it (due to build issues with Qt moc / Qt macros)
namespace openshot
{
    class QtPlayer;
}

class VideoRenderWidget;
using openshot::QtPlayer;

class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

signals:
	void present(const QImage &image);

public slots:
	void open(bool checked = true);
	void open(const QString & file);

private:
    QVBoxLayout *vbox;
    QMenuBar *menu;
    VideoRenderWidget *video;
    QtPlayer *player;
    PlayerControls * controls;
	QString source;

	QTimer * updateTimer = nullptr;
};

#endif // OPENSHOT_PLAYER_H
