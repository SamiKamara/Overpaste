#ifndef SIDEBAR_HH
#define SIDEBAR_HH

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QStyle>
#include <QDebug>
#include <QLabel>

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);
    void toggleSidebar();

private:
    QVBoxLayout *sidebarLayout;
    QPushButton* createButton(const QString& text, const QIcon& icon, const QString& style);
    QPushButton *allFilesButton;
    QPushButton *imagesButton;
    QPushButton *gifsButton;
    QPushButton *textsButton;
    QPushButton *videosButton;
    QPushButton *toggleSidebarButton;
    bool isParentMainWindow;
    QString getTransparency() const;
    QString createLabelStyle();
    bool sideBarMaximized = true;
    QLabel *categoriesLabel;
    QLabel *filesLabel;
signals:
    void imagesButtonClicked();
    void allFilesButtonClicked();
    void gifsButtonClicked();
    void textsButtonClicked();
    void videosButtonClicked();
public slots:
    void updateButtonStyles(int targetFolderId);
};

#endif // SIDEBAR_HH
