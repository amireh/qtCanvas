#ifndef TYPE_EXPORTS_HPP
#define TYPE_EXPORTS_HPP

#include <QObject>
#include <canvas/resources/course.hpp>
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_question.hpp>
#include <canvas/http/response.hpp>
#include <canvas/session.hpp>
#include "include/qproxy.hpp"

typedef QProxy<Canvas::Course> PCourse;
typedef QProxy<Canvas::Quiz> PQuiz;
typedef QProxy<Canvas::QuizQuestion> PQuizQuestion;

Q_DECLARE_METATYPE(PCourse)
Q_DECLARE_METATYPE(PQuiz)
Q_DECLARE_METATYPE(PQuizQuestion)
Q_DECLARE_METATYPE(Canvas::HTTP::Response)
Q_DECLARE_METATYPE(Canvas::Session::RC_GET)

#define QTCANVAS_EXPORT_TYPES \
    qRegisterMetaType<PCourse>("PCourse"); \
    qRegisterMetaType<PQuiz>("PQuiz"); \
    qRegisterMetaType<PQuizQuestion>("PQuizQuestion"); \
    qRegisterMetaType<Canvas::Session::RC_GET>("Canvas::Session::RC_GET"); \
    qRegisterMetaType<Canvas::HTTP::Response>("Canvas::HTTP::Response");

#endif // TYPE_EXPORTS_HPP
