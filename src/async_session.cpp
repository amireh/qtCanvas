#include "include/async_session.hpp"
#include "type_exports.hpp"

using Canvas::Session;
using Canvas::String;
using namespace Canvas::HTTP;


RequestThread::RequestThread(AsyncSession *session,
                             String const uri,
                             Canvas::Session::RC_GET callback) :
    QThread(session),
    Logger("RequestThread"),
    mSession(session),
    mUri(uri),
    mCallback(callback)
{
}

RequestThread::~RequestThread()
{
}

void RequestThread::run() {
    auto notify = [&](bool success, Response const &response) {
        emit done(success, response, QString::fromStdString(mUri), mCallback);
    };

    debug() << "Requesting: " << mUri;
    debug() << "Session address: " << mSession;
    mSession->basePerformRequest(mUri, notify);
}

AsyncSession::AsyncSession() : Session()
{
}

AsyncSession::~AsyncSession()
{
}

void AsyncSession::notify(bool success,
                          const Response &response,
                          QString uri,
                          Session::RC_GET callback)
{
//    mRequestMutex.unlock();

    debug() << "[" << response.status << "] "
            << uri.toStdString()
            << "(" << response.body.size() << " bytes)";

    callback(success, response);
}

bool AsyncSession::performRequest(const String &uri, RC_GET callback)
{
    RequestThread *requestThread;

    mRequestMutex.lock();

    debug() << "dispatching request from: " << QThread::currentThread();

    requestThread = new RequestThread(this, uri, callback);

    connect(requestThread, &RequestThread::done, this, &AsyncSession::notify);
    connect(requestThread, &RequestThread::finished, [&, requestThread]() {
        mRequestMutex.unlock();
        requestThread->deleteLater();
    });

    requestThread->start();

    return true;
}

void AsyncSession::basePerformRequest(const Canvas::String &uri, Session::RC_GET callback)
{
    debug() << "Actually performing request now:" << uri;
    Session::performRequest(uri, callback);
}
