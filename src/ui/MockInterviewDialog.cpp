#include "ui/MockInterviewDialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

MockInterviewDialog::MockInterviewDialog(QuestionRepository *questionRepository, QWidget *parent)
    : QDialog(parent),
      m_questionRepository(questionRepository),
      m_currentIndex(0),
      m_answerVisible(false)
{
    setupUi();
}

void MockInterviewDialog::startInterview()
{
    m_questions = m_questionRepository->randomQuestions(m_categoryCombo->currentText(), m_countSpin->value());
    if (m_questions.isEmpty()) {
        QMessageBox::information(this, "No questions", "No questions found for selected category.");
        return;
    }

    m_currentIndex = 0;
    m_answerVisible = false;
    showCurrentQuestion();
}

void MockInterviewDialog::previousQuestion()
{
    if (m_currentIndex > 0) {
        --m_currentIndex;
        m_answerVisible = false;
        showCurrentQuestion();
    }
}

void MockInterviewDialog::nextQuestion()
{
    if (m_currentIndex < m_questions.size() - 1) {
        ++m_currentIndex;
        m_answerVisible = false;
        showCurrentQuestion();
    }
}

void MockInterviewDialog::toggleAnswer()
{
    if (m_questions.isEmpty()) {
        return;
    }

    m_answerVisible = !m_answerVisible;
    showCurrentQuestion();
}

void MockInterviewDialog::setupUi()
{
    setWindowTitle("Mock Interview");

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->addItems({"All", "Behavioral", "C++", "SQL", "Algorithms", "Data Science", "Other"});

    m_countSpin = new QSpinBox(this);
    m_countSpin->setRange(1, 50);
    m_countSpin->setValue(5);

    auto *startButton = new QPushButton("Start", this);
    connect(startButton, &QPushButton::clicked, this, &MockInterviewDialog::startInterview);

    auto *form = new QFormLayout;
    form->addRow("Category:", m_categoryCombo);
    form->addRow("Number of questions:", m_countSpin);

    auto *topLayout = new QHBoxLayout;
    topLayout->addLayout(form);
    topLayout->addWidget(startButton);

    m_progressLabel = new QLabel("Choose options and press Start.", this);

    m_questionText = new QTextEdit(this);
    m_questionText->setReadOnly(true);
    m_questionText->setMinimumHeight(120);

    m_answerText = new QTextEdit(this);
    m_answerText->setReadOnly(true);
    m_answerText->setMinimumHeight(120);

    m_previousButton = new QPushButton("Previous", this);
    m_nextButton = new QPushButton("Next", this);
    m_showAnswerButton = new QPushButton("Show Answer", this);
    auto *closeButton = new QPushButton("Close", this);

    connect(m_previousButton, &QPushButton::clicked, this, &MockInterviewDialog::previousQuestion);
    connect(m_nextButton, &QPushButton::clicked, this, &MockInterviewDialog::nextQuestion);
    connect(m_showAnswerButton, &QPushButton::clicked, this, &MockInterviewDialog::toggleAnswer);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_previousButton);
    buttonLayout->addWidget(m_nextButton);
    buttonLayout->addWidget(m_showAnswerButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(topLayout);
    layout->addWidget(m_progressLabel);
    layout->addWidget(m_questionText);
    layout->addWidget(m_answerText);
    layout->addLayout(buttonLayout);

    showCurrentQuestion();
    resize(650, 520);
}

void MockInterviewDialog::showCurrentQuestion()
{
    const bool hasQuestions = !m_questions.isEmpty();
    m_previousButton->setEnabled(hasQuestions && m_currentIndex > 0);
    m_nextButton->setEnabled(hasQuestions && m_currentIndex < m_questions.size() - 1);
    m_showAnswerButton->setEnabled(hasQuestions);
    m_showAnswerButton->setText(m_answerVisible ? "Hide Answer" : "Show Answer");

    if (!hasQuestions) {
        m_questionText->clear();
        m_answerText->clear();
        return;
    }

    const InterviewQuestion &question = m_questions[m_currentIndex];
    m_progressLabel->setText(QString("Question %1 of %2 | %3 | %4")
                                 .arg(m_currentIndex + 1)
                                 .arg(m_questions.size())
                                 .arg(question.category(), question.difficulty()));
    m_questionText->setPlainText(question.question());
    m_answerText->setPlainText(m_answerVisible ? question.myAnswer() : "");
}
