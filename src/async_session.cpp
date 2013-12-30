#include "include/async_session.hpp"
#include "type_exports.hpp"

using Canvas::Session;
using Canvas::String;
using namespace Canvas::HTTP;


RequestThread::RequestThread(AsyncSession *session,
                             String const uri,
                             Canvas::Session::RC_GET callback, QMutex &mtx) :
    QThread(session),
    Logger("RequestThread"),
    mSession(session),
    mUri(uri),
    mCallback(callback),
    mMutex(mtx)
{
}

RequestThread::~RequestThread()
{
}

void RequestThread::run() {
    mMutex.lock();

    auto notify = [&](bool success, Response const &response) {
        try {
            emit done(success, response, QString::fromStdString(mUri), mCallback);
        } catch(std::exception const &e) {
            mMutex.unlock();
            throw e;
        }

        mMutex.unlock();
    };


    try {
        debug() << "Requesting: " << mUri;

        mSession->basePerformRequest(mUri, notify);
    } catch(std::exception const &e) {
        mMutex.unlock();
        throw e;
    }
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
    debug() << "[" << response.status << "] "
            << uri.toStdString()
            << "(" << response.body.size() << " bytes)";

    callback(success, response);
}

bool AsyncSession::performRequest(const String &uri, RC_GET callback)
{
    RequestThread *requestThread;

    requestThread = new RequestThread(this, uri, callback, mRequestMutex);

    connect(requestThread, &RequestThread::done, this, &AsyncSession::notify);
    connect(requestThread, &RequestThread::finished, requestThread, &QObject::deleteLater);

    requestThread->start();

    return true;
}

void AsyncSession::basePerformRequest(const Canvas::String &uri, Session::RC_GET callback)
{
    Session::performRequest(uri, callback);
}
