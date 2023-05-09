#include "borderdelegate.hh"
#include <QPainter>

BorderDelegate::BorderDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

void BorderDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (option.state & QStyle::State_MouseOver) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QColor(154, 160, 166, 255), 2));
        painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
        painter->restore();
    }
}
