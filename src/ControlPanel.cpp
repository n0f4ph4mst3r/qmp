#include "ControlPanel.h"

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent)
{
	m_currentTimeLabel = new QLabel();
	m_totalTimeLabel = new QLabel();
	m_slider = new QSlider(Qt::Horizontal, this);
	m_slider->setRange(0, 1);
	connect(m_slider, &QSlider::sliderMoved, this, &ControlPanel::mainSliderMoved);

	QHBoxLayout* durationCtrls_layout = new QHBoxLayout;
	durationCtrls_layout->setContentsMargins(5, 5, 5, 5);
	durationCtrls_layout->addWidget(m_currentTimeLabel);
	durationCtrls_layout->addWidget(m_slider);
	durationCtrls_layout->addWidget(m_totalTimeLabel);

	m_playButton = new QToolButton(this);
	m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	connect(m_playButton, &QToolButton::clicked, this, &ControlPanel::playClicked);

	m_stopButton = new QToolButton(this);
	m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
	connect(m_stopButton, &QToolButton::clicked, this, &ControlPanel::stop);

	m_nextButton = new QToolButton(this);
	m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
	connect(m_nextButton, &QToolButton::clicked, this, &ControlPanel::forward);

	m_previousButton = new QToolButton(this);
	m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
	connect(m_previousButton, &QToolButton::clicked, this, &ControlPanel::backward);

	m_rateBox = new QComboBox(this);
	m_rateBox->addItem("0.25x", QVariant(0.25));
	m_rateBox->addItem("0.5x", QVariant(0.5));
	m_rateBox->addItem("0.75x", QVariant(0.75));
	m_rateBox->addItem("1.0x", QVariant(1.0));
	m_rateBox->addItem("1.25x", QVariant(1.25));
	m_rateBox->addItem("1.5x", QVariant(1.5));
	m_rateBox->addItem("1.75x", QVariant(1.75));
	m_rateBox->addItem("2.0x", QVariant(2.0));
	m_rateBox->setCurrentIndex(3);
	connect(m_rateBox, QOverload<int>::of(&QComboBox::activated), this, &ControlPanel::updateRate);

	m_muteButton = new QToolButton(this);
	m_muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
	connect(m_muteButton, &QToolButton::clicked, this, &ControlPanel::muteClicked);

	m_volumeSlider = new QSlider(Qt::Horizontal, this);
	m_volumeSlider->setRange(0, 100);
	m_volumeSlider->setMaximumWidth(100);
	connect(m_volumeSlider, &QSlider::valueChanged, this, &ControlPanel::volumeChanged);

	QHBoxLayout* layout_sound = new QHBoxLayout;
	layout_sound->setContentsMargins(0, 0, 10, 0);
	layout_sound->addStretch();
	layout_sound->addWidget(m_muteButton);
	layout_sound->addWidget(m_volumeSlider);

	QHBoxLayout* layout_controls = new QHBoxLayout;
	layout_controls->setContentsMargins(5, 10, 5, 15);
	layout_controls->addStretch();
	layout_controls->addWidget(m_stopButton);
	layout_controls->addWidget(m_previousButton);
	layout_controls->addWidget(m_playButton);
	layout_controls->addWidget(m_nextButton);
	layout_controls->addWidget(m_rateBox);
	layout_controls->addLayout(layout_sound);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addLayout(durationCtrls_layout);
	layout->addLayout(layout_controls);
	setLayout(layout);

	m_playerState = QMediaPlayer::StoppedState;
	m_playerMuted = false;
	m_volumeSlider->setValue(100);
	reset();
}

void ControlPanel::reset() {
	if (m_slider->isSliderDown()) m_slider->setSliderDown(false);
	m_slider->setMaximum(0);
	m_currentTimeLabel->setText("--:--");
	m_totalTimeLabel->setText("--:--");

	m_stopButton->setEnabled(false);
	m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void ControlPanel::updateDurationInfo(qint64 currentInfo)
{
	QString currentTimeStr;
	QString totalTimeStr;
	qint64 duration = m_duration / 1000;

    if (currentInfo || duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60, currentInfo % 60,
                          (currentInfo * 1000) % 1000);
        QTime totalTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60,
                        (duration * 1000) % 1000);

		QString format = duration > 3600 ? "hh:mm:ss" : "mm:ss";
		currentTimeStr = currentTime.toString(format);
		totalTimeStr = totalTime.toString(format);
    }

    m_currentTimeLabel->setText(currentTimeStr);
	m_totalTimeLabel->setText(totalTimeStr);
}

void ControlPanel::playClicked()
{
	switch (m_playerState) {
	case QMediaPlayer::StoppedState:
	case QMediaPlayer::PausedState:
		if (m_slider->maximum() == 0)
			m_slider->setMaximum(m_duration);
		emit play();
		break;
	case QMediaPlayer::PlayingState:
		emit pause();
		break;
	}
}

void ControlPanel::muteClicked()
{
	m_playerMuted = !m_playerMuted;
	m_muteButton->setIcon(style()->standardIcon(m_playerMuted ? QStyle::SP_MediaVolumeMuted	: QStyle::SP_MediaVolume));
	emit setMute(m_playerMuted);
}

void ControlPanel::volumeChanged()
{
	qreal linearVolume =
		QAudio::convertVolume(m_volumeSlider->value() / qreal(100),
			QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);

	emit setVolume(linearVolume);
}

QMediaPlayer::PlaybackState ControlPanel::state() const
{
	return m_playerState;
}

void ControlPanel::setState(QMediaPlayer::PlaybackState state)
{
	if (state != m_playerState) {
		m_playerState = state;

		switch (state) {
		case QMediaPlayer::StoppedState:
			reset();
			break;
		case QMediaPlayer::PlayingState:
			m_stopButton->setEnabled(true);
			m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
			break;
		case QMediaPlayer::PausedState:
			m_stopButton->setEnabled(true);
			m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
			break;
		}
	}
}

void ControlPanel::mainSliderMoved(int mseconds)
{
	emit jump(mseconds);
}

void ControlPanel::durationChanged(qint64 duration)
{
	m_duration = duration;
    m_slider->setMaximum(duration);
}

void ControlPanel::positionChanged(qint64 progress)
{
	if (!m_slider->isSliderDown())
        m_slider->setValue(progress);
		
	if (progress > 0)
		updateDurationInfo(progress / 1000);
}

void ControlPanel::setPlaybackRate(float rate)
{
	for (int i = 0; i < m_rateBox->count(); ++i) {
		if (qFuzzyCompare(rate, float(m_rateBox->itemData(i).toDouble()))) {
			m_rateBox->setCurrentIndex(i);
			return;
		}
	}

	m_rateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
	m_rateBox->setCurrentIndex(m_rateBox->count() - 1);
}

void ControlPanel::updateRate()
{
	emit changeRate(m_rateBox->itemData(m_rateBox->currentIndex()).toDouble());
}