#include "include/widgets/question_index.hpp"
#include "ui_question_index.h"

QuestionIndex::QuestionIndex(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionIndex)
{
    ui->setupUi(this);
}

QuestionIndex::~QuestionIndex()
{
    delete ui;
}

void QuestionIndex::render(Quiz::Questions const &questions)
{
    for (auto qq : questions) {
        QListWidgetItem *qqItem = renderQuestionEntry(qq, ui->listWidget);

        if (qqItem) {
            ui->listWidget->addItem(qqItem);
        }
    }
}

QListWidgetItem *QuestionIndex::renderQuestionEntry(const Canvas::QuizQuestion *qq, QListWidget *view)
{
    QListWidgetItem *qqItem;

    qqItem = new QListWidgetItem(QString("Question %1").arg(qq->position()),
                                 view);
    return qqItem;
}
