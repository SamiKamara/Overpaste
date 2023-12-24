#include "keylistener.hh"
#include <QDebug>
#include <QCoreApplication>

KeyListener::KeyListener(QObject *parent)
    : QObject(parent)
{
#ifdef Q_OS_WIN
    // Register the hot key for the '><|' key with the Shift modifier.
    RegisterHotKey(NULL, 1, MOD_SHIFT, VK_OEM_102);
#endif
    QCoreApplication::instance()->installNativeEventFilter(this);
}

KeyListener::~KeyListener()
{
#ifdef Q_OS_WIN
    UnregisterHotKey(NULL, 1);
#endif
    QCoreApplication::instance()->removeNativeEventFilter(this);
}

bool KeyListener::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)

#ifdef Q_OS_WIN
    MSG* msg = static_cast<MSG*>(message);

    if (msg->message == WM_HOTKEY)
    {
        emit hotKeyPressed();
        return true;
    }
#endif

    return false;
}
