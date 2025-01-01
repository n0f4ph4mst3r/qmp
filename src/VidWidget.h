// SPDX-FileCopyrightText: 2024 Korshunov Vladislav <vladredsoup@gmail.com>
// SPDX-License-Identifier: (GPL-3.0-only)

#pragma once

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QKeyEvent>
#include <QMouseEvent>

class VidWidget : public QVideoWidget
{
    Q_OBJECT

public:
    explicit VidWidget(QWidget *parent = nullptr);
    ~VidWidget() = default;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;
};

#endif 