#include "keylistener.hh"
#include <QDebug>
#include <QCoreApplication>

#ifdef Q_OS_WIN
KeyListener* KeyListener::m_instance = nullptr;

LRESULT CALLBACK KeyListener::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_MBUTTONDOWN)
        {
            if (m_instance)
            {
                QMetaObject::invokeMethod(m_instance, "hotKeyPressed", Qt::QueuedConnection);
            }
        }
    }
    return CallNextHookEx(m_instance->m_mouseHook, nCode, wParam, lParam);
}
#endif

KeyListener::KeyListener(QObject *parent)
    : QObject(parent)
{
#ifdef Q_OS_WIN
    m_instance = this;
    if (!RegisterHotKey(NULL, 1, MOD_SHIFT, VK_OEM_102))
    {
        qWarning() << "RegisterHotKey failed!";
    }

    m_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(NULL), 0);

    if (!m_mouseHook)
    {
        qWarning() << "mouse hook failed!";
    }
#endif
    QCoreApplication::instance()->installNativeEventFilter(this);
}

KeyListener::~KeyListener()
{
#ifdef Q_OS_WIN
    UnregisterHotKey(NULL, 1);
    if (m_mouseHook)
    {
        UnhookWindowsHookEx(m_mouseHook);
    }
    m_instance = nullptr;
#endif
    QCoreApplication::instance()->removeNativeEventFilter(this);
}

bool KeyListener::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)

#ifdef Q_OS_WIN
    MSG* msg = static_cast<MSG*>(message);

    //if (msg->message == WM_HOTKEY)
    //{
    //    emit hotKeyPressed();
    //    return true;
    //}
#endif

    return false;
}
