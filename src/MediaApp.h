// SPDX-FileCopyrightText: 2024 Korshunov Vladislav <vladredsoup@gmail.com>
// SPDX-License-Identifier: (GPL-3.0-only)

#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMediaFormat>
#include <QMimeType>
#include <QStringList>
#include <QSet>
#include <QUrl>

#include "ControlPanel.h"
#include "VidWidget.h"

class MediaApp : public QMainWindow
{
	Q_OBJECT
private:
	QWidget* m_mainWidget;
	ControlPanel* m_mediaCtrl;
	VidWidget* m_videoWidget;
	QMediaPlayer* m_player;
	QAudioOutput* m_audioOutput;
	QSet<QUrl> recentFiles;

	void about();
	void open();
signals:
	void fileOpened();
public:
	explicit MediaApp(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~MediaApp() = default;
};

#endif