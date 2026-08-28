#include "ui/QuestionDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QVBoxLayout>

QuestionDialog::QuestionDialog(int applicationId, QWidget *parent)
    : QDialog(parent),
      m_id(-1),
      m_applicationId(applicationId)
{
    setupUi();
    setWindowTitle("Add Question");
}

QuestionDialog::QuestionDialog(const InterviewQuestion &question, QWidget *parent)
    : QDialog(parent),
      m_id(question.id()),
      m_applicationId(question.applicationId())
{
    setupUi();
    setWindowTitle("Edit Question");
    loadQuestion(question);
}

InterviewQuestion QuestionDialog::question() const
{
    return InterviewQuestion(m_id,
                             m_applicationId,
                             m_questionEdit->toPlainText().trimmed(),
                             m_categoryCombo->currentText(),
                             m_difficultyCombo->currentText(),
                             m_answerEdit->toPlainText().trimmed(),
                             m_notesEdit->toPlainText().trimmed(),
                             m_practicedCheck->isChecked());
}

void QuestionDialog::setupUi()
{
    m_questionEdit = new QTextEdit(this);
    m_questionEdit->setMinimumHeight(80);

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->addItems({"Behavioral", "C++", "SQL", "Algorithms", "Data Science", "Other"});

    m_difficultyCombo = new QComboBox(this);
    m_difficultyCombo->addItems({"Easy", "Medium", "Hard"});

    m_answerEdit = new QTextEdit(this);
    m_answerEdit->setMinimumHeight(90);

    m_notesEdit = new QTextEdit(this);
    m_notesEdit->setMinimumHeight(70);

    m_practicedCheck = new QCheckBox("Practiced", this);

    auto *form = new QFormLayout;
    form->addRow("Question:", m_questionEdit);
    form->addRow("Category:", m_categoryCombo);
    form->addRow("Difficulty:", m_difficultyCombo);
    form->addRow("My Answer:", m_answerEdit);
    form->addRow("Notes:", m_notesEdit);
    form->addRow("", m_practicedCheck);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (m_questionEdit->toPlainText().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Missing data", "Question text is required.");
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
    resize(520, 520);
}

void QuestionDialog::loadQuestion(const InterviewQuestion &question)
{
    m_questionEdit->setPlainText(question.question());
    m_categoryCombo->setCurrentText(question.category());
    m_difficultyCombo->setCurrentText(question.difficulty());
    m_answerEdit->setPlainText(question.myAnswer());
    m_notesEdit->setPlainText(question.notes());
    m_practicedCheck->setChecked(question.practiced());
}
