#include "MediaDropArea.hh"

void MediaDropArea::createShortcut(const QString &sourcePath, const QString &shortcutPath) {
    QFile::link(sourcePath, shortcutPath);
}

MediaDropArea::MediaDropArea(QWidget *parent) : QLabel(parent) {
    setAcceptDrops(true);
    setAlignment(Qt::AlignCenter);
    QFont font = QFont("Arial", 20);
    setFont(font);
    setText("");
    setFrameStyle(QFrame::Box | QFrame::Sunken);
    setStyleSheet("background-color: rgba(0, 0, 0, 0); color: rgb(154, 160, 166);");
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void MediaDropArea::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        setStyleSheet("background-color: rgba(154, 160, 166, 50); color: rgb(154, 160, 166);");
        setText("Drop file here, to save as pasta");
        event->acceptProposedAction();      
    }
}

void MediaDropArea::dragLeaveEvent(QDragLeaveEvent *event) {
    setStyleSheet("background-color: rgba(0, 0, 0, 0); color: rgb(154, 160, 166);");
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setText("");
    event->accept();
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

                QTimer::singleShot(3000, this, &MediaDropArea::resetInfoMessage);

                event->accept();

                break;
            }
        }

        if (!fileSupported) {
            displayUnsupportedFileMessage();
        }

        setAttribute(Qt::WA_TransparentForMouseEvents);
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
    setStyleSheet("background-color: rgba(0, 0, 0, 0); color: rgb(154, 160, 166);");
    setText("");
}

void MediaDropArea::displayUnsupportedFileMessage() {
    setText("Unsupported file type.");
    QTimer::singleShot(3000, this, &MediaDropArea::resetInfoMessage);
}

