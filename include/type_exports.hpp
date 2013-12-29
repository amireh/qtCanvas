#ifndef TYPE_EXPORTS_HPP
#define TYPE_EXPORTS_HPP

#include <QObject>
#include <canvas/resources/course.hpp>
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_question.hpp>
#include "include/qproxy.hpp"

typedef QProxy<Canvas::Course> PCourse;
typedef QProxy<Canvas::Quiz> PQuiz;
typedef QProxy<Canvas::QuizQuestion> PQuizQuestion;

Q_DECLARE_METATYPE(PCourse)
Q_DECLARE_METATYPE(PQuiz)
Q_DECLARE_METATYPE(PQuizQuestion)

#define QTCANVAS_EXPORT_TYPES \
    qRegisterMetaType<PCourse>("PCourse"); \
    qRegisterMetaType<PQuiz>("PQuiz"); \
    qRegisterMetaType<PQuiz>("PQuizQuestion");

#endif // TYPE_EXPORTS_HPP
