

#include "Qt/PlayerControls.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QAudio>
#include <QLabel>
#include <QTimer>

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
    , playerState(QMediaPlayer::StoppedState)
    , playerMuted(false)
{

    auto openfile = new QToolButton(this);
    openfile->setIcon(style()->standardIcon(QStyle::SP_FileDialogStart));
    connect(openfile, SIGNAL(clicked(bool)), this, SIGNAL(open()));

    playButton = new QToolButton(this);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

	saveButton = new QToolButton(this);
	saveButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	connect(saveButton, SIGNAL(clicked()), this, SIGNAL(save()));

    stopButton = new QToolButton(this);
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setEnabled(false);

    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));

    nextButton = new QToolButton(this);
    nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(nextButton, SIGNAL(clicked()), this, SIGNAL(next()));

    previousButton = new QToolButton(this);
    previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));

    muteButton = new QToolButton(this);
    muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    connect(muteButton, SIGNAL(clicked()), this, SLOT(muteClicked()));

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);

    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(onVolumeSliderValueChanged()));

    rateBox = new QComboBox(this);
    rateBox->addItem("0.5x", QVariant(0.5));
    rateBox->addItem("1.0x", QVariant(1.0));
	rateBox->addItem("2.0x", QVariant(2.0));
	rateBox->addItem("4.0x", QVariant(4.0));
	rateBox->addItem("8.0x", QVariant(8.0));
    rateBox->setCurrentIndex(1);

    connect(rateBox, SIGNAL(activated(int)), SLOT(updateRate()));

    QBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(openfile);

	saveButton->hide();
	layout->addWidget(saveButton);

    layout->addWidget(stopButton);

	previousButton->hide();
    layout->addWidget(previousButton);
    layout->addWidget(playButton);
	nextButton->hide();
    layout->addWidget(nextButton);

	muteButton->hide();
    layout->addWidget(muteButton);

	volumeSlider->hide();
    layout->addWidget(volumeSlider);

	rateBox->hide();
    layout->addWidget(rateBox);

	positionSlider = new QSlider(Qt::Horizontal, this);
	//positionSlider->setTracking(false);
	//connect(this, SIGNAL(valueChanged(int)), this, SLOT(onPositionSliderValueChanged()));
	connect(positionSlider, SIGNAL(sliderPressed()), this, SLOT(onPositionSliderValueChanged()));
	connect(positionSlider, SIGNAL(sliderReleased()), this, SLOT(onPositionSliderValueChanged()));
	connect(positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(onPositionSliderValueChanged()));


	layout->addWidget(positionSlider);

	positionLabel = new QLabel(this);
	positionLabel->setLineWidth(100);
	layout->addWidget(positionLabel);

    setLayout(layout);
}

QMediaPlayer::State PlayerControls::state() const
{
    return playerState;
}

void PlayerControls::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
            stopButton->setEnabled(false);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}


int PlayerControls::position() const
{
	return positionSlider->value();
}

int PlayerControls::volume() const
{
    qreal linearVolume =  QAudio::convertVolume(volumeSlider->value() / qreal(100),
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}

void PlayerControls::setVolume(int volume)
{
    qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100),
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);

    volumeSlider->setValue(qRound(logarithmicVolume * 100));
}


bool PlayerControls::isMuted() const
{
    return playerMuted;
}

void PlayerControls::setMuted(bool muted)
{
    if (muted != playerMuted) {
        playerMuted = muted;

        muteButton->setIcon(style()->standardIcon(muted
                ? QStyle::SP_MediaVolumeMuted
                : QStyle::SP_MediaVolume));
    }
}

void PlayerControls::setPosition(int pos) {
	if (!positionSlider->isSliderDown()) {
		positionSlider->blockSignals(true);
		positionSlider->setValue(pos);
		positionSlider->blockSignals(false);
	}
	positionLabel->setText(QString("%1/%2/%3")
		.arg(pos)
		.arg(positionSlider->maximum())
		.arg(positionSlider->value())
	);
}

void PlayerControls::setRange(int start, int end) {
	positionSlider->blockSignals(true);
	positionSlider->setRange(start, end);
	positionSlider->blockSignals(false);
}

void PlayerControls::playClicked()
{
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void PlayerControls::muteClicked()
{
    emit changeMuting(!playerMuted);
}

qreal PlayerControls::playbackRate() const
{
    return rateBox->itemData(rateBox->currentIndex()).toDouble();
}

void PlayerControls::setPlaybackRate(float rate)
{
    for (int i = 0; i < rateBox->count(); ++i) {
        if (qFuzzyCompare(rate, float(rateBox->itemData(i).toDouble()))) {
            rateBox->setCurrentIndex(i);
            return;
        }
    }

    rateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
    rateBox->setCurrentIndex(rateBox->count() - 1);
}

void PlayerControls::updateRate()
{
    emit changeRate(playbackRate());
}

void PlayerControls::onVolumeSliderValueChanged()
{
	emit changeVolume(volume());
}

void PlayerControls::onPositionSliderValueChanged()
{
	emit changePosition(position());
}
