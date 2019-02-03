

#include "stdio.h"
#include "QtPlayer.h"
#include "Qt/PlayerWidget.h"
#include <QMessageBox>
#include <QFileDialog>

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
    , vbox(new QVBoxLayout(this))
    , menu(new QMenuBar(this))
    , video(new VideoRenderWidget(this))
    , player(new QtPlayer(video->GetRenderer()))
{
    setWindowTitle("OpenShot Player");

    menu->setNativeMenuBar(false);

    QAction *action = NULL;
    action = menu->addAction("Choose File");
    connect(action, SIGNAL(triggered(bool)), this, SLOT(open(bool)));

    controls = new PlayerControls(this);
	controls->setMaximumHeight(20);
    vbox->addWidget(video);
	vbox->addWidget(controls);

    vbox->setMargin(0);
    vbox->setSpacing(0);
    resize(600, 480);

    // Accept keyboard event
    setFocusPolicy(Qt::StrongFocus);
	controls->setFocusPolicy(Qt::NoFocus);

	connect(video->GetRenderer(), SIGNAL(present(const QImage &)), this, SIGNAL(present(const QImage &)));

	{
		connect(controls, &PlayerControls::open, [this]() {
			this->open(true);
			//player->SetSource("C:/Users/tony/Videos/zhifou-2.mp4");
			//player->Play();
		});
		//connect(controls, SIGNAL(save()), this, SLOT(save()));
		connect(controls, &PlayerControls::play,  [this]() {
			if (player->Mode() == openshot::PLAYBACK_STOPPED && !source.isEmpty()) {
				player->SetSource(source.toStdString());
				player->Play();
				controls->setState(QMediaPlayer::PlayingState);
			}
			else if (player->Mode() == openshot::PLAYBACK_PAUSED) {
				player->Play();
				controls->setState(QMediaPlayer::PlayingState);
			}
		});
		connect(controls, &PlayerControls::pause, [this]() {
			player->Pause();
			controls->setState(QMediaPlayer::PausedState);
		});
		connect(controls, &PlayerControls::stop,  [this]() {
			player->Stop();
			controls->setState(QMediaPlayer::StoppedState);
		});

		connect(controls, &PlayerControls::changeRate, [this](qreal rate) {
			player->Speed(rate);
		});
		//connect(controls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
		//connect(controls, SIGNAL(changeMuting(bool)), player, SLOT(setMuted(bool)));
		//connect(controls, SIGNAL(changeRate(qreal)), player, SLOT(setPlaybackRate(qreal)));

		connect(controls, &PlayerControls::changePosition, [this](int pos) {
			player->Seek(pos);
		});
	}

	updateTimer = new QTimer(this);
	updateTimer->setInterval(80);
	updateTimer->setSingleShot(false);
	connect(updateTimer, &QTimer::timeout, [this]() {
		auto reader = player->Reader();
		if (reader) {
			controls->setRange(0, reader->info.video_length);
			controls->setPosition(player->Position());
		}
	});
	updateTimer->start();
}

PlayerWidget::~PlayerWidget()
{
	player->Stop();
	delete player;
}

void PlayerWidget::closeEvent(QCloseEvent *event)
{
	// Close window, stop player, and quit
	QWidget *pWin = QApplication::activeWindow();
	pWin->hide();
	player->Stop();
	//QApplication::quit();
}

void PlayerWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space || event->key() == Qt::Key_K) {

		if (player->Mode() == openshot::PLAYBACK_PAUSED)
		{
			// paused, so start playing again
			player->Play();

		}
		else if (player->Mode() == openshot::PLAYBACK_PLAY)
		{

			if (player->Speed() == 0)
				// already playing, but speed is zero... so just speed up to normal
				player->Speed(1);
			else
				// already playing... so pause
				player->Pause();

		}

	}
	else if (event->key() == Qt::Key_J) {
		cout << "BACKWARD" << player->Speed() - 1 << endl;
		if (player->Speed() - 1 != 0)
			player->Speed(player->Speed() - 1);
		else
			player->Speed(player->Speed() - 2);

		if (player->Mode() == openshot::PLAYBACK_PAUSED)
			player->Play();
	}
	else if (event->key() == Qt::Key_L) {
		cout << "FORWARD" << player->Speed() + 1 << endl;
		if (player->Speed() + 1 != 0)
			player->Speed(player->Speed() + 1);
		else
			player->Speed(player->Speed() + 2);

		if (player->Mode() == openshot::PLAYBACK_PAUSED)
			player->Play();

	}
	else if (event->key() == Qt::Key_Left) {
		cout << "FRAME STEP -1" << endl;
		if (player->Speed() != 0)
			player->Speed(0);
		player->Seek(player->Position() - 1);
	}
	else if (event->key() == Qt::Key_Right) {
		cout << "FRAME STEP +1" << endl;
		if (player->Speed() != 0)
			player->Speed(0);
		player->Seek(player->Position() + 1);
	}
	else if (event->key() == Qt::Key_Escape) {
		cout << "QUIT PLAYER" << endl;
		QWidget *pWin = QApplication::activeWindow();
		pWin->hide();

		player->Stop();

		//QApplication::quit();
	}

	event->accept();
	QWidget::keyPressEvent(event);
}

void PlayerWidget::open(bool checked)
{
	// Get filename of media files
    const QString filename = QFileDialog::getOpenFileName(this, "Open Video File");
	open(filename);
}


void PlayerWidget::open(const QString & filename) {
	
	if (filename.isEmpty()) 
		return;

	source = filename;

    // Create FFmpegReader and open file
    player->SetSource(filename.toStdString());

    // Set aspect ratio of widget
    video->SetAspectRatio(player->Reader()->info.display_ratio, player->Reader()->info.pixel_ratio);

    // Play video
    player->Play(); 
	controls->setState(QMediaPlayer::PlayingState);

	auto reader = player->Reader();
	if (reader) {
		auto info = reader->info;
		controls->setRange(0, info.video_length);
		controls->setPosition(0);
	}
}