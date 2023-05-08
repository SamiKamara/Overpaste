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
    mutable QCache<QString, QIcon> m_iconCache{500};
};

#endif // CUSTOMICONFILESYSTEMMODEL_HH
