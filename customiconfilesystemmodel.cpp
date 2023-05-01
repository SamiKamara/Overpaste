#include "customiconfilesystemmodel.hh"
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QSize>

CustomIconFileSystemModel::CustomIconFileSystemModel(QObject *parent) : QFileSystemModel(parent) {
}

QVariant CustomIconFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DecorationRole && index.column() == 0) {
        QString filePath = QFileSystemModel::data(index, QFileSystemModel::FilePathRole).toString();
        QString resolvedPath = Explorer::resolveShortcut(filePath);

        QIcon thumbnailIcon = generateThumbnail(resolvedPath);
        return thumbnailIcon;
    }

    return QFileSystemModel::data(index, role);
}

QIcon CustomIconFileSystemModel::generateThumbnail(const QString &filePath) const {
    static const int thumbnailSize = 96;

    QImageReader reader(filePath);
    if (!reader.canRead()) {
        return QIcon::fromTheme("unknown");
    }

    reader.setScaledSize(QSize(thumbnailSize, thumbnailSize));

    QImage thumbnail = reader.read();
    if (thumbnail.isNull()) {
        return QIcon::fromTheme("unknown");
    }

    QPixmap pixmap = QPixmap::fromImage(thumbnail);
    QIcon icon(pixmap);
    return icon;
}
