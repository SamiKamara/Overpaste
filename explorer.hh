#ifndef EXPLORER_HH
#define EXPLORER_HH

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>
#include <QListView>
#include <QClipboard>
#include <QMimeData>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#ifdef Q_OS_WIN
    #include <windows.h>
    #include <shlobj.h>
    #include <shobjidl.h>
    #include <objbase.h>
#endif

class Explorer : public QWidget
{
    Q_OBJECT

public:
    explicit Explorer(QWidget *parent = nullptr);
    int targetFolderId;

private:
    QFileSystemModel *m_model;
    QListView *m_listView;

    QString resolveShortcut(const QString &filePath);
    void updateModelRootPath();
private slots:
    void onFileClicked(const QModelIndex &index);

public slots:
    void onImagesButtonClicked();
    void onAllFilesButtonClicked();
    void onGifsButtonClicked();
    void onTextsButtonClicked();
    void onVideosButtonClicked();
};

#endif // EXPLORER_HH
