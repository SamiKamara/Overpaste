#ifndef CUSTOMICONFILESYSTEMMODEL_HH
#define CUSTOMICONFILESYSTEMMODEL_HH

#include <QFileSystemModel>
#include <QCoreApplication>
#include <QIcon>
#include <QPainter>
#include <QCache>

#include "explorer.hh"

class CustomIconFileSystemModel : public QFileSystemModel {
    Q_OBJECT

public:
    CustomIconFileSystemModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QIcon generateThumbnail(const QString &filePath) const;
    QPixmap createThumbnailPixmap(const QString &filePath) const;
    QPixmap createWhitePixmap(const QString &fileSuffix, int thumbnailSize) const;
    void setPainterFont(QPainter &painter, int fontSize) const;
    QString createContentToDisplay(const QString &fileSuffix, const QFileInfo &fileInfo, const QString &filePath) const;
    void drawTextOnPixmap(QPainter &painter, const QString &contentToDisplay, const QColor &textColor, int thumbnailSize) const;
    void drawFileNameOnThumbnail(QPainter &painter, const QString &fileName, int thumbnailSize) const;
    int calculateFontSize(const QString &fileName) const;
    QPixmap createPixmapFromImage(const QString &filePath, int thumbnailSize) const;

    mutable QCache<QString, QIcon> m_iconCache{500};
};

#endif // CUSTOMICONFILESYSTEMMODEL_HH

