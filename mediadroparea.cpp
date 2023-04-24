#include "MediaDropArea.hh"

MediaDropArea::MediaDropArea(QWidget *parent) : QLabel(parent) {
    setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    setText("Drop .png file here, to save as pasta.png");
    setFrameStyle(QFrame::Box | QFrame::Sunken);
}

void MediaDropArea::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MediaDropArea::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        for (const QUrl &url : urlList) {
            QFileInfo fileInfo(url.toLocalFile());
            QString fileSuffix = fileInfo.suffix().toLower();

            if (isSupportedFileFormat(fileSuffix)) {
                QString originalFilename = fileInfo.fileName();
                QString targetFolderPath = getTargetFolderPath(fileSuffix);
                QString buildPath = targetFolderPath + "/" + originalFilename;

                createFolderIfNotExists(targetFolderPath);
                copyFileToPath(url.toLocalFile(), buildPath);

                if (fileSuffix == "png") {
                    QString pastaPath = getImagesFolderPath() + "/pasta." + fileSuffix;
                    copyFileToPath(url.toLocalFile(), pastaPath);
                }

                setText(getSavedMediaFileMessage(originalFilename, fileSuffix));
                break;
            }
        }
    }
}

bool MediaDropArea::isSupportedFileFormat(const QString &fileSuffix) {
    return (fileSuffix == "png"     ||
            fileSuffix == "jpg"     ||
            fileSuffix == "jpeg"    ||
            fileSuffix == "gif"     ||
            fileSuffix == "bmp"     ||
            fileSuffix == "mp4");
}

QString MediaDropArea::getTargetFolderPath(const QString &fileSuffix) {
    if (fileSuffix == "gif") {
        return QApplication::applicationDirPath() + "/gifs";
    } else if (fileSuffix == "mp4") {
        return QApplication::applicationDirPath() + "/videos";
    } else {
        return getImagesFolderPath();
    }
}

QString MediaDropArea::getImagesFolderPath() {
    return QApplication::applicationDirPath() + "/images";
}

void MediaDropArea::createFolderIfNotExists(const QString &folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void MediaDropArea::copyFileToPath(const QString &sourcePath, const QString &destinationPath) {
    QFile oldFile(destinationPath);
    if (oldFile.exists()) {
        oldFile.remove();
    }

    QFile file(sourcePath);
    file.copy(destinationPath);
}

QString MediaDropArea::getSavedMediaFileMessage(const QString &filename, const QString &fileSuffix) {
    QString folderName;
    if (fileSuffix == "gif") {
        folderName = "gifs";
    } else if (fileSuffix == "mp4") {
        folderName = "videos";
    } else {
        folderName = "images";
    }

    QString message = "File saved as " + filename + " in " + folderName + " folder";

    if (fileSuffix == "png") {
        message += " and pasta." + fileSuffix + " in Images folder";
    }

    return message;
}
