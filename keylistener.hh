#ifndef KEYLISTENER_HH
#define KEYLISTENER_HH

#include <QObject>
#include <QAbstractNativeEventFilter>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class KeyListener : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit KeyListener(QObject *parent = nullptr);
    ~KeyListener();

    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

signals:
    void hotKeyPressed();
};

#endif // KEYLISTENER_HH
