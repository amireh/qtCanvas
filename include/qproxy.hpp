#ifndef QPROXY_HPP
#define QPROXY_HPP

template<class T>
class QProxy {
public:
    QProxy() : mObject(nullptr) {}
    QProxy(T* object) : mObject(object) {}
    QProxy(T const* object) : mObject((T*)object) {}

    QProxy(QProxy<T> const& rhs) {
        mObject = rhs.mObject;
    }

    virtual ~QProxy() {
        mObject = nullptr;
    }

    T* object() {
        return mObject;
    }
    T const* object() const {
        return mObject;
    }

    T* operator*() {
        return mObject;
    }

    operator bool() const {
        return mObject != nullptr;
    }

private:
    T* mObject;
};

#endif // QPROXY_HPP
