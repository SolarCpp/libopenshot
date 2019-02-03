

#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
class QLabel;
class QTimer;
QT_END_NAMESPACE

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    PlayerControls(QWidget *parent = 0);

    QMediaPlayer::State state() const;
    int volume() const;
	int position() const;
    bool isMuted() const;
    qreal playbackRate() const;

public slots:
    void setState(QMediaPlayer::State state);
    void setVolume(int volume);
    void setMuted(bool muted);
    void setPlaybackRate(float rate);
	void setPosition(int pos);
	void setRange(int start, int end);

signals:
    void open();
	void save();
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeMuting(bool muting);
    void changeRate(qreal rate);
	void changePosition(int position);

private slots:
    void playClicked();
    void muteClicked();
    void updateRate();
	void onVolumeSliderValueChanged();
	void onPositionSliderValueChanged();

private:
    QMediaPlayer::State playerState;
    bool playerMuted;
	QAbstractButton *playButton = nullptr;
	QAbstractButton *saveButton = nullptr;
    QAbstractButton *stopButton = nullptr;
    QAbstractButton *nextButton = nullptr;
    QAbstractButton *previousButton = nullptr;
    QAbstractButton *muteButton = nullptr;
	QAbstractSlider *volumeSlider = nullptr;
	QAbstractSlider *positionSlider = nullptr;
	bool positionChangingpositionChanging = false;
	QLabel * positionLabel = nullptr;
    QComboBox *rateBox = nullptr;
};

#endif // PLAYERCONTROLS_H
