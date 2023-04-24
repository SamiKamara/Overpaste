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
            if (fileInfo.suffix().toLower() == "png") {
                QFile file(url.toLocalFile());
                QString originalFilename = fileInfo.fileName(); // Get the original filename

                QString imagesFolderPath = QApplication::applicationDirPath() + "/Images"; // Get the "Images" folder path in the current save location
                QDir imagesDir(imagesFolderPath);
                if (!imagesDir.exists()) {
                    imagesDir.mkpath("."); // Create the "Images" folder if it doesn't exist
                }

                QString buildPath = imagesFolderPath + "/" + originalFilename; // Save the file in the "Images" folder
                QFile oldFile(buildPath);
                if (oldFile.exists()) {
                    oldFile.remove();
                }
                file.copy(buildPath); // Save the file with its original name

                QString pastaPath = imagesFolderPath + "/pasta.png";
                QFile pastaFile(pastaPath);
                if (pastaFile.exists()) {
                    pastaFile.remove();
                }
                file.copy(pastaPath); // Save another copy of the file as "pasta.png"

                setText("File saved as " + originalFilename + " and pasta.png in Images folder"); // Display the original filename and "pasta.png" in the message
                break;
            }
        }
    }
}
