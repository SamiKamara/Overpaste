#ifndef SIDEBAR_HH
#define SIDEBAR_HH

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget *parent = nullptr);

private:
    QVBoxLayout *sidebarLayout;
    QPushButton* createButton(const QString& text, const QIcon& icon, const QString& style);

signals:
    void imagesButtonClicked();
    void allFilesButtonClicked();
    void gifsButtonClicked();
    void textsButtonClicked();
    void videosButtonClicked();
};

#endif // SIDEBAR_HH
