#include "customiconfilesystemmodel.hh"
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QSize>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

CustomIconFileSystemModel::CustomIconFileSystemModel(QObject *parent) : QFileSystemModel(parent) {
}

QVariant CustomIconFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DecorationRole && index.column() == 0) {
        QString filePath = QFileSystemModel::data(index, QFileSystemModel::FilePathRole).toString();
        QString resolvedPath = Explorer::resolveShortcut(filePath);

        QIcon *cachedIcon = m_iconCache.object(resolvedPath);
        if (cachedIcon) {
            return *cachedIcon;
        }

        QIcon thumbnailIcon = generateThumbnail(resolvedPath);
        m_iconCache.insert(resolvedPath, new QIcon(thumbnailIcon));
        return thumbnailIcon;
    }

    if (role == Qt::DisplayRole) {
        return QString("");
    }

    return QFileSystemModel::data(index, role);
}

QIcon CustomIconFileSystemModel::generateThumbnail(const QString &filePath) const {
    static const int thumbnailSize = 100;

    QFileInfo fileInfo(filePath);
    QString fileSuffix = fileInfo.suffix().toLower();

    QPixmap whitePixmap(thumbnailSize, thumbnailSize);

    if (fileSuffix == "txt") {
        whitePixmap.fill(QColor(25, 25, 25, 255));
    } else {
        whitePixmap.fill(QColor(244, 244, 244, 255));
    }

    QPainter painter(&whitePixmap);
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);

    if (fileSuffix == "mp4" || fileSuffix == "txt") {
        QStringList fileNameChunks;
        QString contentToDisplay;
        QColor textColor;

        if (fileSuffix == "txt") {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                QTextStream in(&file);
                contentToDisplay = in.read(80);
                file.close();
            }
            textColor = QColor(154, 160, 166, 255);
        } else {
            contentToDisplay = "Video:\n" + fileInfo.completeBaseName();
            textColor = Qt::black;
        }

        for (int i = 0; i < contentToDisplay.length(); i += 14) {
            fileNameChunks << contentToDisplay.mid(i, 14);
        }
        QString multiLineFileName = fileNameChunks.join("\n");

        painter.setPen(textColor);
        QRect textRect = QRect(0, 0, thumbnailSize, thumbnailSize);
        painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, multiLineFileName);
    }

    QString fileName = fileInfo.fileName();
    QRect bottomTextRect = QRect(0, thumbnailSize - 20, thumbnailSize, 20);

    QRect barRect(bottomTextRect);
    painter.setBrush(QColor(0, 0, 0, 180));
    painter.setPen(Qt::NoPen);
    painter.drawRect(barRect);

    QFont fileNameFont = painter.font();

    int fontSize = 10;
    if (fileName.length() > 18) {
        fontSize = static_cast<int>(10.0 * 16.0 / fileName.length());
        if (fontSize < 1) {
            fontSize = 1;
        }
    }

    fileNameFont.setPointSize(fontSize);
    painter.setFont(fileNameFont);
    painter.setPen(QColor(233, 233, 233, 255));
    painter.drawText(bottomTextRect, Qt::AlignCenter, fileName);

    QImageReader reader(filePath);
    if (!reader.canRead()) {
        return QIcon(whitePixmap);
    }

    reader.setScaledSize(QSize(thumbnailSize, thumbnailSize));

    QImage thumbnail = reader.read();
    if (thumbnail.isNull()) {
        return QIcon(whitePixmap);
    }

    QPixmap pixmap = QPixmap::fromImage(thumbnail);

    QPainter pixmapPainter(&pixmap);
    pixmapPainter.setFont(font);

    pixmapPainter.setBrush(QColor(0, 0, 0, 180));
    pixmapPainter.setPen(Qt::NoPen);
    pixmapPainter.drawRect(barRect);

    pixmapPainter.setFont(fileNameFont);
    pixmapPainter.setPen(QColor(233, 233, 233, 255));
    pixmapPainter.drawText(bottomTextRect, Qt::AlignCenter, fileName);

    QIcon icon(pixmap);
    return icon;
}




