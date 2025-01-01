// SPDX-FileCopyrightText: 2024 Korshunov Vladislav <vladredsoup@gmail.com>
// SPDX-License-Identifier: (GPL-3.0-only)

#include "MediaApp.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

#ifndef Q_OS_WINDOWS
    QApplication::setWindowIcon(QIcon(":/icons/app_icon.xpm"));
#endif

    MediaApp player;
    player.show();
    player.resize(1280, 720);

    return app.exec();
}