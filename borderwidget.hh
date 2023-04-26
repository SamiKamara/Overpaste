#ifndef BORDERWIDGET_HH
#define BORDERWIDGET_HH

#include <QWidget>
#include <QPaintEvent>

class BorderWidget : public QWidget {
public:
    explicit BorderWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // BORDERWIDGET_HH
