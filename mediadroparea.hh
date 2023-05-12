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
#include <QTimer>

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
    bool isSupportedFileFormat(const QString &fileSuffix);
    QString getSavedMediaFileMessage(const QString &filename, const QString &fileSuffix);
    void createShortcut(const QString &sourcePath, const QString &shortcutPath);
    void resetInfoMessage();
    void displayUnsupportedFileMessage();
};

#endif // MEDIAROPAREA_H
