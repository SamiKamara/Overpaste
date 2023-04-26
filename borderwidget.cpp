#include "borderwidget.hh"
#include <QPainter>
#include <QPen>

BorderWidget::BorderWidget(QWidget *parent)
    : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
}

void BorderWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen(Qt::black, 1);
    painter.setPen(pen);
    painter.drawRect(0, 0, width() - pen.width(), height() - pen.width());
}
