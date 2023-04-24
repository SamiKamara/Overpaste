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
private:
    QString getTargetFolderPath(const QString &fileSuffix);
    QString getImagesFolderPath();
    void createFolderIfNotExists(const QString &folderPath);
    void copyFileToPath(const QString &sourcePath, const QString &destinationPath);
    //QString getSavedFileMessage(const QString &filename, const QString &fileSuffix);
    bool isSupportedFileFormat(const QString &fileSuffix);
    QString getSavedMediaFileMessage(const QString &filename, const QString &fileSuffix);
};

#endif // MEDIAROPAREA_H
