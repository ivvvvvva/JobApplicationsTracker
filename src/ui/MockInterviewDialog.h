#ifndef MOCKINTERVIEWDIALOG_H
#define MOCKINTERVIEWDIALOG_H

#include "models/InterviewQuestion.h"
#include "repositories/QuestionRepository.h"

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

class MockInterviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MockInterviewDialog(QuestionRepository *questionRepository, QWidget *parent = nullptr);

private slots:
    void startInterview();
    void previousQuestion();
    void nextQuestion();
    void toggleAnswer();

private:
    void setupUi();
    void showCurrentQuestion();

    QuestionRepository *m_questionRepository;
    QComboBox *m_categoryCombo;
    QSpinBox *m_countSpin;
    QLabel *m_progressLabel;
    QTextEdit *m_questionText;
    QTextEdit *m_answerText;
    QPushButton *m_previousButton;
    QPushButton *m_nextButton;
    QPushButton *m_showAnswerButton;
    QList<InterviewQuestion> m_questions;
    int m_currentIndex;
    bool m_answerVisible;
};

#endif
