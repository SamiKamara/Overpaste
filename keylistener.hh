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

#ifdef Q_OS_WIN
private:
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static KeyListener* m_instance;
    HHOOK m_mouseHook;
#endif
};

#endif // KEYLISTENER_HH
