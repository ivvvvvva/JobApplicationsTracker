#ifndef APPLICATIONDETAILSDIALOG_H
#define APPLICATIONDETAILSDIALOG_H

#include "models/JobApplication.h"
#include "models/InterviewQuestion.h"
#include "repositories/QuestionRepository.h"

#include <QLabel>
#include <QList>
#include <QTableWidget>
#include <QDialog>

class ApplicationDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    ApplicationDetailsDialog(const JobApplication &application,
                             QuestionRepository *questionRepository,
                             QWidget *parent = nullptr);

private slots:
    void addQuestion();
    void editQuestion();
    void deleteQuestion();
    void updateQuestionPreview();

private:
    void setupUi();
    void loadQuestions();
    InterviewQuestion selectedQuestion() const;
    int selectedQuestionId() const;

    JobApplication m_application;
    QuestionRepository *m_questionRepository;
    QLabel *m_infoLabel;
    QLabel *m_questionPreviewLabel;
    QLabel *m_answerPreviewLabel;
    QLabel *m_notesPreviewLabel;
    QTableWidget *m_questionsTable;
    QList<InterviewQuestion> m_questions;
};

#endif
