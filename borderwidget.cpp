#include "borderwidget.hh"
#include <QPainter>
#include <QPen>
#include <QColor>

BorderWidget::BorderWidget(QWidget *parent)
    : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
}

void BorderWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen(QColor(44, 49, 58), 1);
    painter.setPen(pen);
    painter.drawRect(0, 0, width() - pen.width(), height() - pen.width());
}
