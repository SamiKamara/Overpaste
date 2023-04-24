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
                QString buildPath = QApplication::applicationDirPath() + "/pasta.png";
                QFile oldFile(buildPath);
                if (oldFile.exists()) {
                    oldFile.remove();
                }
                file.copy(buildPath);
                setText("File saved as pasta.png");
                break;
            }
        }
    }
}
