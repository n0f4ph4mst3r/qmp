#include "MediaApp.h"

MediaApp::MediaApp(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), m_mainWidget(new QWidget)
{
	setCentralWidget(m_mainWidget);
	setWindowTitle(QObject::tr("Qmp"));

	m_player = new QMediaPlayer;
	m_audioOutput = new QAudioOutput;
	m_player->setAudioOutput(m_audioOutput);

	m_videoWidget = new VidWidget(m_mainWidget);
	m_videoWidget->resize(1280, 720);
	m_player->setVideoOutput(m_videoWidget);

	m_mediaCtrl = new ControlPanel(m_mainWidget);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_videoWidget, 2);
	layout->addWidget(m_mediaCtrl);

	QMenu* mainMenu = menuBar()->addMenu(QObject::tr("&File"));
	mainMenu->addAction(QObject::tr("&Open"), [&]() { open(); })->setShortcut(Qt::Key_O | Qt::CTRL);
	QMenu* recentFileMenu = mainMenu->addMenu(QObject::tr("&Recent files"));
	recentFileMenu->setDisabled(true);
	connect(this, &MediaApp::fileOpened, recentFileMenu, [=]() {recentFileMenu->setDisabled(false);
																recentFileMenu->clear();
																for (const auto &value : recentFiles)
																	recentFileMenu->addAction(value.path(), [&]() { m_player->setSource(value.path());
																													m_mediaCtrl->setState(QMediaPlayer::PlayingState);
																													m_player->play(); });});
	mainMenu->addSeparator();

	mainMenu->addAction(QObject::tr("&Fullscreen"), [&]() {m_videoWidget->setFullScreen(!m_videoWidget->isFullScreen()); })->setShortcut(Qt::Key_F11);
	mainMenu->addSeparator();
	mainMenu->addAction(QObject::tr("&Quit"), []() { qApp->quit(); })->setShortcut(Qt::Key_Q | Qt::CTRL);;

	QMenu* helpMenu = menuBar()->addMenu(QObject::tr("&Help"));
	helpMenu->addAction(QIcon(":/icons/app_icon.png"), QObject::tr("About"), [&]() { about(); })->setShortcut(Qt::Key_F1 | Qt::SHIFT);
	helpMenu->addAction(QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton), QObject::tr("About Qt"), []() { qApp->aboutQt(); });

	m_mainWidget->setLayout(layout);
	m_mainWidget->setMinimumSize(640, 480);

	connect(m_mediaCtrl, &ControlPanel::play, m_player, &QMediaPlayer::play);
	connect(m_mediaCtrl, &ControlPanel::pause, m_player, &QMediaPlayer::pause);
	connect(m_mediaCtrl, &ControlPanel::stop, m_player, &QMediaPlayer::stop);
	connect(m_mediaCtrl, &ControlPanel::forward, [&]() { m_player->setPosition(m_player->position() + 5000);});
	connect(m_mediaCtrl, &ControlPanel::backward, [&]() { m_player->setPosition(m_player->position() - 5000);});

	connect(m_mediaCtrl, &ControlPanel::jump, m_player, &QMediaPlayer::setPosition);
	connect(m_player, &QMediaPlayer::durationChanged, m_mediaCtrl, &ControlPanel::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, m_mediaCtrl, &ControlPanel::positionChanged);

	connect(m_mediaCtrl, &ControlPanel::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
	connect(m_mediaCtrl, &ControlPanel::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));
	connect(m_player, &QMediaPlayer::playbackStateChanged, m_mediaCtrl, &ControlPanel::setState);

	connect(m_mediaCtrl, &ControlPanel::setVolume, m_audioOutput, &QAudioOutput::setVolume);
	connect(m_mediaCtrl, &ControlPanel::setMute, m_audioOutput, &QAudioOutput::setMuted);

	m_videoWidget->show();
	this->installEventFilter(this);
}

void MediaApp::about()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.setText(tr("### Qmp\r\n"
		"Simple multimedia player that can play audio and or video files using various codecs.\r\n"
		"#### Author\r\n"
		"Korshunov Vladislav \\<vladredsoup@gmail.com\\>\n\n"
		"[License](https://github.com/n0f4ph4mst3r/qmp/blob/master/LICENSE.txt) | [GitHub](https://github.com/n0f4ph4mst3r/qmp)"));
	msgBox.setIconPixmap(QPixmap(":/icons/app_icon.png"));
	msgBox.exec();
}

void MediaApp::open()
{
	QMediaFormat defaultFormat;

	QList<QMediaFormat::FileFormat> supMediaFormats = defaultFormat.supportedFileFormats(QMediaFormat::Decode);
	QStringList supportedMedia;
	for (auto item : supMediaFormats) {
		QMediaFormat mediaFormat;
		mediaFormat.setFileFormat(item);
		supportedMedia.append(mediaFormat.mimeType().globPatterns());
	}
	supportedMedia.removeDuplicates();
	QString supMediaFilter = QString("Media files (%1)").arg(supportedMedia.join(' '));

	QList<QMediaFormat::VideoCodec> supVideoFormats = defaultFormat.supportedVideoCodecs(QMediaFormat::Decode);
	QStringList supportedVideo;
	for (auto item : supVideoFormats) {
		QMediaFormat mediaFormat;
		mediaFormat.setVideoCodec(item);
		mediaFormat.resolveForEncoding(QMediaFormat::RequiresVideo);
		supportedVideo.append(mediaFormat.mimeType().globPatterns());
	}
	supportedVideo.removeDuplicates();
	QString supVideoFilter = QString("Video files (%1)").arg(supportedVideo.join(' '));

	QList<QMediaFormat::AudioCodec> supAudioFormats = defaultFormat.supportedAudioCodecs(QMediaFormat::Decode);
	QStringList supportedAudio;
	for (auto item : supAudioFormats) {
		QMediaFormat mediaFormat;
		mediaFormat.setAudioCodec(item);
		mediaFormat.resolveForEncoding(QMediaFormat::NoFlags);
		supportedAudio.append(mediaFormat.mimeType().globPatterns());
	}
	supportedAudio.removeDuplicates();
	QString supAudioFilter = QString("Audio files (%1)").arg(supportedAudio.join(' '));

	QStringList nameFilters;
	nameFilters.append(supMediaFilter);
	nameFilters.append(supVideoFilter);
	nameFilters.append(supAudioFilter);
	nameFilters.append("All files (*)");

	QFileDialog dialog(m_mainWidget, tr("Open File"),
		QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilters(nameFilters);

	if (dialog.exec()) {
		auto path = dialog.selectedFiles().last();

		if (!path.isEmpty()) {
			if (!recentFiles.contains(path) && recentFiles.count() >= 5)
				recentFiles.erase(recentFiles.begin());
			recentFiles.insert(path);
			emit fileOpened();

			m_player->setSource(path);
			m_mediaCtrl->setState(QMediaPlayer::PlayingState);
			m_player->play();
		}
	}
}
