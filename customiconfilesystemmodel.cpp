#include "customiconfilesystemmodel.hh"
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QSize>
#include <QFileInfo>

CustomIconFileSystemModel::CustomIconFileSystemModel(QObject *parent) : QFileSystemModel(parent) {
}

QVariant CustomIconFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DecorationRole && index.column() == 0) {
        QString filePath = QFileSystemModel::data(index, QFileSystemModel::FilePathRole).toString();
        QString resolvedPath = Explorer::resolveShortcut(filePath);

        QIcon thumbnailIcon = generateThumbnail(resolvedPath);
        return thumbnailIcon;
    }

    if (role == Qt::ForegroundRole) {
        return QColor(154, 160, 166);
    }

    return QFileSystemModel::data(index, role);
}


QIcon CustomIconFileSystemModel::generateThumbnail(const QString &filePath) const {
    static const int thumbnailSize = 200;

    QPixmap whitePixmap(thumbnailSize, thumbnailSize);
    whitePixmap.fill(Qt::white);

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.completeBaseName();

    if (fileInfo.suffix().toLower() == "mp4") {
        QStringList fileNameChunks;
        for (int i = 0; i < fileName.length(); i += 14) {
            fileNameChunks << fileName.mid(i, 14);
        }
        QString multiLineFileName = fileNameChunks.join("\n");

        QPainter painter(&whitePixmap);
        QFont font = painter.font();
        font.setPointSize(20);
        painter.setFont(font);
        painter.setPen(Qt::black);
        QRect textRect = QRect(0, 0, thumbnailSize, thumbnailSize);
        painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, multiLineFileName);
        return QIcon(whitePixmap);
    }

    QImageReader reader(filePath);
    if (!reader.canRead()) {
        return QIcon::fromTheme("unknown");
    }

    reader.setScaledSize(QSize(thumbnailSize, thumbnailSize));

    QImage thumbnail = reader.read();
    if (thumbnail.isNull()) {
        return QIcon(whitePixmap);
    }

    QPixmap pixmap = QPixmap::fromImage(thumbnail);
    QIcon icon(pixmap);
    return icon;
}


