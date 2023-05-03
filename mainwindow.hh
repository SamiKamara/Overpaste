#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QClipboard>
#include <QImage>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QFileInfo>
#include <QFile>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <sidebar.hh>
#include <QSplitter>
#include <mediadroparea.hh>
#include <borderwidget.hh>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:

private:
    QPoint m_dragPosition;
    BorderWidget* m_borderWidget;
    bool m_isDragging;
    void onDragFinished();
};

#endif // MAINWINDOW_HH
