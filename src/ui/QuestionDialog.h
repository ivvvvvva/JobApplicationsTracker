#ifndef QUESTIONDIALOG_H
#define QUESTIONDIALOG_H

#include "models/InterviewQuestion.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QTextEdit>

class QuestionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionDialog(int applicationId, QWidget *parent = nullptr);
    explicit QuestionDialog(const InterviewQuestion &question, QWidget *parent = nullptr);

    InterviewQuestion question() const;

private:
    void setupUi();
    void loadQuestion(const InterviewQuestion &question);

    int m_id;
    int m_applicationId;
    QTextEdit *m_questionEdit;
    QComboBox *m_categoryCombo;
    QComboBox *m_difficultyCombo;
    QTextEdit *m_answerEdit;
    QTextEdit *m_notesEdit;
    QCheckBox *m_practicedCheck;
};

#endif
