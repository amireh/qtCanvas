#ifndef TYPE_EXPORTS_HPP
#define TYPE_EXPORTS_HPP

#include <QObject>
#include <canvas/resources/quiz.hpp>
#include "include/qproxy.hpp"

typedef QProxy<Canvas::Course> PCourse;
typedef QProxy<Canvas::Quiz> PQuiz;

Q_DECLARE_METATYPE(PCourse)
Q_DECLARE_METATYPE(PQuiz)

#define QTCANVAS_EXPORT_TYPES \
    qRegisterMetaType<PCourse>("PCourse"); \
    qRegisterMetaType<PQuiz>("PQuiz");

#endif // TYPE_EXPORTS_HPP
