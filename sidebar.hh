#ifndef SIDEBAR_HH
#define SIDEBAR_HH

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QStyle>
#include <QDebug>

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);

private:
    QVBoxLayout *sidebarLayout;
    QPushButton* createButton(const QString& text, const QIcon& icon, const QString& style);
    QPushButton *allFilesButton;
    QPushButton *imagesButton;
    QPushButton *gifsButton;
    QPushButton *textsButton;
    QPushButton *videosButton;

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
