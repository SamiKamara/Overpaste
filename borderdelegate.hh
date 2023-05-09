#ifndef BORDERDELEGATE_HH
#define BORDERDELEGATE_HH

#include <QStyledItemDelegate>

class BorderDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BorderDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // BORDERDELEGATE_HH
