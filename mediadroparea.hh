#ifndef MEDIAROPAREA_H
#define MEDIAROPAREA_H

#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QFile>
#include <QApplication>
#include <QDir>

class MediaDropArea : public QLabel {
public:
    MediaDropArea(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // MEDIAROPAREA_H
