#include "explorer.hh"
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QClipboard>
#include <QGuiApplication>

Explorer::Explorer(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_listView = new QListView(this);
    layout->addWidget(m_listView);
    layout->setContentsMargins(0, 0, 0, 0);

    m_model = new QFileSystemModel(this);
    m_model->setRootPath(QCoreApplication::applicationDirPath() + "/files");
    m_model->setFilter(QDir::Files);
    m_model->setHeaderData(0, Qt::Horizontal, "File Explorer");

    m_listView->setModel(m_model);
    m_listView->setRootIndex(m_model->index(QCoreApplication::applicationDirPath() + "/files"));
    m_listView->setViewMode(QListView::IconMode);
    m_listView->setGridSize(QSize(100, 100));
    m_listView->setSpacing(10);
    m_listView->setResizeMode(QListView::Adjust);
    m_listView->setUniformItemSizes(true);
    m_listView->setMovement(QListView::Static);

    this->setLayout(layout);
    connect(m_listView, &QListView::clicked, this, &Explorer::onFileClicked);
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

