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
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLabel>
#include "mainwindow.hh"
#include <QDebug>

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
    explicit Explorer(MainWindow* window, QWidget *parent = nullptr);
    int targetFolderId;
    static QString resolveShortcut(const QString &filePath);

private:
    QFileSystemModel *m_model;
    QListView *m_listView;
    QLabel *m_emptyLabel;
    void updateModelRootPath();
    MainWindow* window;
private slots:
    void onFileClicked(const QModelIndex &index);
    void onDirectoryLoaded(const QString &);

public slots:
    void onImagesButtonClicked();
    void onAllFilesButtonClicked();
    void onGifsButtonClicked();
    void onTextsButtonClicked();
    void onVideosButtonClicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void targetFolderChanged(int targetFolderId);
};

#endif // EXPLORER_HH
