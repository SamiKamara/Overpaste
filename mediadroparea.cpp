#include "MediaDropArea.hh"

void MediaDropArea::createShortcut(const QString &sourcePath, const QString &shortcutPath) {
    QFile::link(sourcePath, shortcutPath);
}

MediaDropArea::MediaDropArea(QWidget *parent) : QLabel(parent) {
    setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    setText("Drop file here, to save as pasta");
    setFrameStyle(QFrame::Box | QFrame::Sunken);
    setStyleSheet("color: rgb(154, 160, 166);");
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
        bool fileSupported = false;

        for (const QUrl &url : urlList) {
            QFileInfo fileInfo(url.toLocalFile());
            QString fileSuffix = fileInfo.suffix().toLower();

            if (isSupportedFileFormat(fileSuffix)) {
                fileSupported = true;

                QString originalFilename = fileInfo.fileName();
                QString targetFolderPath = getTargetFolderPath(fileSuffix);
                QString buildPath = targetFolderPath + "/" + originalFilename;

                createFolderIfNotExists(targetFolderPath);
                copyFileToPath(url.toLocalFile(), buildPath);

                setText(getSavedMediaFileMessage(originalFilename, fileSuffix));

                QTimer::singleShot(4000, this, &MediaDropArea::resetInfoMessage);
                break;
            }
        }

        if (!fileSupported) {
            displayUnsupportedFileMessage();
        }
    }
}

bool MediaDropArea::isSupportedFileFormat(const QString &fileSuffix) {
    return (fileSuffix == "png"     ||
            fileSuffix == "jpg"     ||
            fileSuffix == "jpeg"    ||
            fileSuffix == "gif"     ||
            fileSuffix == "bmp"     ||
            fileSuffix == "txt"     ||
            fileSuffix == "mp4");
}

QString MediaDropArea::getTargetFolderPath(const QString &fileSuffix) {
    QString targetFolderPath = QApplication::applicationDirPath();

    if (fileSuffix == "gif") {
        targetFolderPath += "/files/gifs";
    } else if (fileSuffix == "mp4") {
        targetFolderPath += "/files/videos";
    } else if (fileSuffix == "txt") {
        targetFolderPath += "/files/texts";
    } else {
        targetFolderPath += "/files/images";
    }

    return targetFolderPath;
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

    QFileInfo fileInfo(destinationPath);
    QDir parentFolder = fileInfo.dir().absolutePath() + "/..";

    QString libraryFolderPath = parentFolder.absolutePath() + "/library";
    createFolderIfNotExists(libraryFolderPath);

    QString shortcutPath = libraryFolderPath + "/" + fileInfo.fileName() + ".lnk";

    createShortcut(destinationPath, shortcutPath);
}

QString MediaDropArea::getSavedMediaFileMessage(const QString &filename, const QString &fileSuffix) {
    QString folderName;
    if (fileSuffix == "gif") {
        folderName = "gifs";
    } else if (fileSuffix == "mp4") {
        folderName = "videos";
    } else if (fileSuffix == "txt") {
        folderName = "texts";
    } else {
        folderName = "images";
    }

    QString message = "File saved as " + filename + " in " + folderName + " folder";
    return message;
}

void MediaDropArea::resetInfoMessage() {
    setText("Drop file here, to save as pasta");
}

void MediaDropArea::displayUnsupportedFileMessage() {
    setText("Unsupported file type.");
    QTimer::singleShot(3000, this, &MediaDropArea::resetInfoMessage);
}
