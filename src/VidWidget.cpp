#include "VidWidget.h"

VidWidget::VidWidget(QWidget *parent) : QVideoWidget(parent)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);

    installEventFilter(this);
}

void VidWidget::keyPressEvent(QKeyEvent *event)
{
    // if ((event->key() == Qt::Key_Escape || event->key() == Qt::Key_Back) && !isFullScreen()) {
    //     setFullScreen(false);
    //     event->accept();
    // } else if (event->key() == Qt::Key_F11) {
    //     setFullScreen(!isFullScreen());
    //     event->accept();
    // } else {
    //     QVideoWidget::keyPressEvent(event);
    // }
    QVideoWidget::keyPressEvent(event);
}

void VidWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    setFullScreen(!isFullScreen());
    event->accept();
}

void VidWidget::mousePressEvent(QMouseEvent *event)
{
    QVideoWidget::mousePressEvent(event);
}

bool VidWidget::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if ((key == Qt::Key_Escape || key == Qt::Key_Back || Qt::Key_F11) && isFullScreen()) {
            setFullScreen(false);
            return true;
        } 
    }
    return QVideoWidget::eventFilter(object, event);
}

