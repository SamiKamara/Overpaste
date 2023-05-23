#include "customiconfilesystemmodel.hh"
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QSize>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include "explorer.hh"

CustomIconFileSystemModel::CustomIconFileSystemModel(MainWindow* window, QObject *parent)
    : QFileSystemModel(parent), window(window) {
}

QVariant CustomIconFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DecorationRole && index.column() == 0) {
        QString filePath = QFileSystemModel::data(index, QFileSystemModel::FilePathRole).toString();
        QString resolvedPath = Explorer::resolveShortcut(filePath);

        QFileInfo fileInfo(resolvedPath);
        QString fileName = fileInfo.fileName();

        QIcon *cachedIcon = m_iconCache.object(fileName);
        if (cachedIcon) {
            return *cachedIcon;
        }

        QIcon thumbnailIcon = generateThumbnail(resolvedPath);
        m_iconCache.insert(fileName, new QIcon(thumbnailIcon));
        return thumbnailIcon;
    }

    if (role == Qt::DisplayRole) {
        return QString("");
    }

    return QFileSystemModel::data(index, role);
}


QIcon CustomIconFileSystemModel::generateThumbnail(const QString &filePath) const {
    QPixmap thumbnailPixmap = createThumbnailPixmap(filePath);

    if (window->isFullscreenOn()) {
        thumbnailPixmap = applyTransparency(thumbnailPixmap, 100);
    }

    QIcon icon(thumbnailPixmap);
    return icon;
}

QPixmap CustomIconFileSystemModel::createThumbnailPixmap(const QString &filePath) const {
    int thumbnailSize = 100;

    if(window->isFullscreenOn()){
        thumbnailSize *= 2;
    }

    QFileInfo fileInfo(filePath);
    QString fileSuffix = fileInfo.suffix().toLower();

    QPixmap pixmap = createPixmapFromImage(filePath, thumbnailSize);
    if (pixmap.isNull()) {
        QPixmap whitePixmap = createWhitePixmap(fileSuffix, thumbnailSize);
        pixmap = whitePixmap;
    }

    QPainter painter(&pixmap);
    setPainterFont(painter, 10);

    if (fileSuffix == "mp4" || fileSuffix == "txt") {
        QString contentToDisplay = createContentToDisplay(fileSuffix, fileInfo, filePath);
        QColor textColor = (fileSuffix == "txt") ? QColor(154, 160, 166, 255) : Qt::black;

        drawTextOnPixmap(painter, contentToDisplay, textColor, thumbnailSize);
    }

    drawFileNameOnThumbnail(painter, fileInfo.fileName(), thumbnailSize);

    return pixmap;
}


QPixmap CustomIconFileSystemModel::createWhitePixmap(const QString &fileSuffix, int thumbnailSize) const {
    QPixmap whitePixmap(thumbnailSize, thumbnailSize);

    if (fileSuffix == "txt") {
        whitePixmap.fill(QColor(25, 25, 25, 255));
    } else {
        whitePixmap.fill(QColor(244, 244, 244, 255));
    }

    return whitePixmap;
}

void CustomIconFileSystemModel::setPainterFont(QPainter &painter, int fontSize) const {
    QFont font = painter.font();
    font.setPointSize(fontSize);
    painter.setFont(font);
}

QString CustomIconFileSystemModel::createContentToDisplay(const QString &fileSuffix, const QFileInfo &fileInfo, const QString &filePath) const {
    QString contentToDisplay;

    if (fileSuffix == "txt") {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            contentToDisplay = in.read(80);
            file.close();
        }
    } else {
        contentToDisplay = "Video:\n" + fileInfo.completeBaseName();
    }

    return contentToDisplay;
}

void CustomIconFileSystemModel::drawTextOnPixmap(QPainter &painter, const QString &contentToDisplay, const QColor &textColor, int thumbnailSize) const {
    QStringList fileNameChunks;

    for (int i = 0; i < contentToDisplay.length(); i += 14) {
        fileNameChunks << contentToDisplay.mid(i, 14);
    }
    QString multiLineFileName = fileNameChunks.join("\n");

    painter.setPen(textColor);
    QRect textRect = QRect(0, 0, thumbnailSize, thumbnailSize);
    painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, multiLineFileName);
}

void CustomIconFileSystemModel::drawFileNameOnThumbnail(QPainter &painter, const QString &fileName, int thumbnailSize) const {
    QRect bottomTextRect = QRect(0, thumbnailSize - 20, thumbnailSize, 20);

    if(window->isFullscreenOn()){
        bottomTextRect = QRect(0, thumbnailSize - 20 * 2, thumbnailSize, 20 * 2);
    }

    QRect barRect(bottomTextRect);
    painter.setBrush(QColor(0, 0, 0, 180));
    painter.setPen(Qt::NoPen);
    painter.drawRect(barRect);

    QFont fileNameFont = painter.font();
    int fontSize = calculateFontSize(fileName);

    fileNameFont.setPointSize(fontSize);
    painter.setFont(fileNameFont);
    painter.setPen(QColor(233, 233, 233, 255));
    painter.drawText(bottomTextRect, Qt::AlignCenter, fileName);
}

int CustomIconFileSystemModel::calculateFontSize(const QString &fileName) const {
    double fontSize = 10.0;

    if(window->isFullscreenOn()){
        fontSize *= 2;
    }

    if (fileName.length() > 18) {
        fontSize = static_cast<int>(fontSize * 16.0 / fileName.length());
        if (fontSize < 1) {
            fontSize = 1;
        }
    }
    return fontSize;
}

QPixmap CustomIconFileSystemModel::createPixmapFromImage(const QString &filePath, int thumbnailSize) const {
    QImageReader reader(filePath);
    if (!reader.canRead()) {
        return QPixmap();
    }

    reader.setScaledSize(QSize(thumbnailSize, thumbnailSize));

    QImage thumbnail = reader.read();
    if (thumbnail.isNull()) {
        return QPixmap();
    }

    QPixmap pixmap = QPixmap::fromImage(thumbnail);
    return pixmap;
}

QPixmap CustomIconFileSystemModel::applyTransparency(const QPixmap &pixmap, int alpha) const {
    QPixmap transparentPixmap(pixmap.size());
    transparentPixmap.fill(Qt::transparent);

    QPainter painter(&transparentPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(transparentPixmap.rect(), QColor(0, 0, 0, alpha));
    painter.end();

    return transparentPixmap;
}





