// SPDX-FileCopyrightText: 2024 Korshunov Vladislav <vladredsoup@gmail.com>
// SPDX-License-Identifier: (GPL-3.0-only)

#pragma once

#ifndef PLRCONTROL_H
#define PLRCONTROL_H

#include <utility>

#include <QWidget>
#include <QMediaPlayer>
#include <QSlider>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QComboBox>
#include <QTime>

class ControlPanel : public QWidget
{
	Q_OBJECT
private:
    QMediaPlayer::PlaybackState m_playerState;
    bool m_playerMuted;

    QSlider* m_slider;
    QLabel* m_currentTimeLabel;
    QLabel* m_totalTimeLabel;
    qint64 m_duration;

    QToolButton* m_playButton;
    QToolButton* m_stopButton;
    QToolButton* m_nextButton;
    QToolButton* m_previousButton;
    QToolButton* m_muteButton;
    QSlider* m_volumeSlider;
    QComboBox* m_rateBox;

    void reset();
    void updateDurationInfo(qint64 currentInfo);
signals:
    void play();
    void pause();
    void stop();
    void jump(int mseconds);
    void forward();
    void backward();
    void setVolume(float volume);
    void setMute(bool muting);
    void changeRate(qreal rate);

private slots:
    void playClicked();
    void muteClicked();
    void volumeChanged();
    void updateRate();

public:
	explicit ControlPanel(QWidget* parent = nullptr);
    ~ControlPanel() = default;

    QMediaPlayer::PlaybackState state() const;

public slots:
    void setState(QMediaPlayer::PlaybackState state);

    void mainSliderMoved(int mseconds);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void setPlaybackRate(float rate);
};

#endif