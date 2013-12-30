#ifndef ASYNC_SESSION_HPP
#define ASYNC_SESSION_HPP

#include <QObject>
#include <QThread>
#include <QMutex>
#include <canvas/session.hpp>
#include <canvas/http/response.hpp>
#include "type_exports.hpp"

class RequestThread;
class AsyncSession : public QObject, public Canvas::Session
{
    Q_OBJECT
public:
    AsyncSession();
    virtual ~AsyncSession();

private slots:
    void notify(bool, Canvas::HTTP::Response const&, QString, Canvas::Session::RC_GET);
protected:
    friend class RequestThread;
    virtual bool performRequest(Canvas::String const&, Session::RC_GET);
    void basePerformRequest(Canvas::String const&, Session::RC_GET);
    QMutex mRequestMutex;
};

class RequestThread : public QThread, public Canvas::Logger {
    Q_OBJECT
public:
    RequestThread(AsyncSession *s,
                  Canvas::String const inUri,
                  Canvas::Session::RC_GET,
                  QMutex&);
    virtual ~RequestThread();

    void run() Q_DECL_OVERRIDE;

    AsyncSession *mSession;
    Canvas::String const mUri;
    Canvas::Session::RC_GET mCallback;
    QMutex &mMutex;
signals:
    void done(bool,
              Canvas::HTTP::Response const&,
              QString,
              Canvas::Session::RC_GET);

protected:
};

#endif // ASYNC_SESSION_HPP
