#include "explorer.hh"
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QClipboard>
#include <QGuiApplication>
#include <QDir>

Explorer::Explorer(QWidget *parent)
    : QWidget(parent)
{
    targetFolderId = 0;
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_listView = new QListView(this);
    layout->addWidget(m_listView);
    layout->setContentsMargins(0, 0, 0, 0);

    m_model = new CustomIconFileSystemModel(this);
    m_model->setFilter(QDir::Files);
    m_model->setHeaderData(0, Qt::Horizontal, "File Explorer");

    m_listView->setModel(m_model);
    m_listView->setViewMode(QListView::IconMode);
    m_listView->setGridSize(QSize(100, 100));
    m_listView->setSpacing(2);
    m_listView->setResizeMode(QListView::Adjust);
    m_listView->setUniformItemSizes(false);
    m_listView->setMovement(QListView::Static);

    this->setLayout(layout);
    connect(m_listView, &QListView::clicked, this, &Explorer::onFileClicked);

    updateModelRootPath();
}

void Explorer::updateModelRootPath()
{
    QString rootPath;

    if (targetFolderId == 0)
    {
        rootPath = QCoreApplication::applicationDirPath() + "/files/library";
    }
    else if (targetFolderId == 1)
    {
        rootPath = QCoreApplication::applicationDirPath() + "/files/images";
    }
    else if (targetFolderId == 2)
    {
        rootPath = QCoreApplication::applicationDirPath() + "/files/gifs";
    }
    else if (targetFolderId == 3)
    {
        rootPath = QCoreApplication::applicationDirPath() + "/files/texts";
    }
    else if (targetFolderId == 4)
    {
        rootPath = QCoreApplication::applicationDirPath() + "/files/videos";
    }

    QDir dir;
    if (!dir.exists(rootPath))
    {
        dir.mkpath(rootPath);
    }

    m_model->setRootPath(rootPath);
    m_listView->setRootIndex(m_model->index(rootPath));
}

void Explorer::onFileClicked(const QModelIndex &index)
{
    QString filePath = m_model->filePath(index);
    QString resolvedPath = resolveShortcut(filePath);
    QUrl fileUrl = QUrl::fromLocalFile(resolvedPath);

    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls(QList<QUrl>() << fileUrl);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setMimeData(mimeData);
}


QString Explorer::resolveShortcut(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    // Handling Windows shortcuts
    #ifdef Q_OS_WIN
        if (fileInfo.suffix().toLower() == "lnk")
        {
            QString targetPath;
            CoInitialize(NULL);
            IShellLink *pShellLink;
            HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pShellLink);

            if (SUCCEEDED(hres))
            {
                IPersistFile *pPersistFile;
                hres = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID *)&pPersistFile);

                if (SUCCEEDED(hres))
                {
                    hres = pPersistFile->Load((LPCOLESTR)filePath.utf16(), STGM_READ);
                    if (SUCCEEDED(hres))
                    {
                        WIN32_FIND_DATA wfd;
                        WCHAR wsz[MAX_PATH];
                        hres = pShellLink->GetPath(wsz, MAX_PATH, (WIN32_FIND_DATA *)&wfd, SLGP_UNCPRIORITY);
                        if (SUCCEEDED(hres))
                        {
                            targetPath = QString::fromUtf16(reinterpret_cast<const char16_t*>(wsz));

                        }
                    }
                    pPersistFile->Release();
                }
                pShellLink->Release();
            }
            CoUninitialize();

            return targetPath.isEmpty() ? filePath : targetPath;
        }
    #endif

    //#ifdef Q_OS_LINUX
    //    if (fileInfo.isSymLink()) {
    //        return fileInfo.symLinkTarget();
    //    }
    //#endif

    return filePath;
}

void Explorer::onAllFilesButtonClicked()
{
    targetFolderId = 0;
    updateModelRootPath();
}

void Explorer::onImagesButtonClicked()
{
    targetFolderId = 1;
    updateModelRootPath();
}

void Explorer::onGifsButtonClicked()
{
    targetFolderId = 2;
    updateModelRootPath();
}

void Explorer::onTextsButtonClicked()
{
    targetFolderId = 3;
    updateModelRootPath();
}

void Explorer::onVideosButtonClicked()
{
    targetFolderId = 4;
    updateModelRootPath();
}
