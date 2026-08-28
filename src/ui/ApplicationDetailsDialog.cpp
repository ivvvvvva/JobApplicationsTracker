#include "ui/ApplicationDetailsDialog.h"

#include "ui/QuestionDialog.h"

#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>

ApplicationDetailsDialog::ApplicationDetailsDialog(const JobApplication &application,
                                                   QuestionRepository *questionRepository,
                                                   QWidget *parent)
    : QDialog(parent),
      m_application(application),
      m_questionRepository(questionRepository)
{
    setupUi();
    loadQuestions();
}

void ApplicationDetailsDialog::addQuestion()
{
    QuestionDialog dialog(m_application.id(), this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    InterviewQuestion question = dialog.question();
    if (!m_questionRepository->add(question)) {
        QMessageBox::critical(this, "Database error", m_questionRepository->lastError());
        return;
    }

    loadQuestions();
}

void ApplicationDetailsDialog::editQuestion()
{
    const int id = selectedQuestionId();
    if (id < 0) {
        QMessageBox::information(this, "Select question", "Please select a question first.");
        return;
    }

    QuestionDialog dialog(selectedQuestion(), this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    if (!m_questionRepository->update(dialog.question())) {
        QMessageBox::critical(this, "Database error", m_questionRepository->lastError());
        return;
    }

    loadQuestions();
}

void ApplicationDetailsDialog::deleteQuestion()
{
    const int id = selectedQuestionId();
    if (id < 0) {
        QMessageBox::information(this, "Select question", "Please select a question first.");
        return;
    }

    const auto answer = QMessageBox::question(this, "Delete question", "Delete selected question?");
    if (answer != QMessageBox::Yes) {
        return;
    }

    if (!m_questionRepository->remove(id)) {
        QMessageBox::critical(this, "Database error", m_questionRepository->lastError());
        return;
    }

    loadQuestions();
}

void ApplicationDetailsDialog::updateQuestionPreview()
{
    const InterviewQuestion question = selectedQuestion();
    if (question.id() < 0) {
        m_questionPreviewLabel->setText("Select a question to see full details.");
        m_answerPreviewLabel->setText("-");
        m_notesPreviewLabel->setText("-");
        return;
    }

    m_questionPreviewLabel->setText(question.question().toHtmlEscaped());
    m_answerPreviewLabel->setText(question.myAnswer().isEmpty()
                                      ? "-"
                                      : question.myAnswer().toHtmlEscaped());
    m_notesPreviewLabel->setText(question.notes().isEmpty()
                                     ? "-"
                                     : question.notes().toHtmlEscaped());
}

void ApplicationDetailsDialog::setupUi()
{
    setWindowTitle("Application Details");

    m_infoLabel = new QLabel(this);
    m_infoLabel->setText(QString("<b>%1</b> - %2<br>Status: %3<br>Applied: %4<br>URL: %5<br><br>%6")
                             .arg(m_application.company().toHtmlEscaped(),
                                  m_application.position().toHtmlEscaped(),
                                  m_application.status().toHtmlEscaped(),
                                  m_application.dateApplied().toHtmlEscaped(),
                                  m_application.jobUrl().toHtmlEscaped(),
                                  m_application.notes().toHtmlEscaped()));
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setObjectName("infoPanel");

    m_questionsTable = new QTableWidget(this);
    m_questionsTable->setColumnCount(5);
    m_questionsTable->setHorizontalHeaderLabels({"ID", "Question", "Category", "Difficulty", "Practiced"});
    m_questionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_questionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_questionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_questionsTable->setAlternatingRowColors(true);
    m_questionsTable->setWordWrap(false);
    m_questionsTable->verticalHeader()->setVisible(false);
    m_questionsTable->hideColumn(0);
    m_questionsTable->horizontalHeader()->setStretchLastSection(false);
    m_questionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_questionsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_questionsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_questionsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_questionsTable->setMinimumWidth(430);

    auto *preview = new QWidget(this);
    preview->setObjectName("previewPanel");
    auto *previewLayout = new QVBoxLayout(preview);

    auto *previewTitle = new QLabel("Question Details", preview);
    previewTitle->setObjectName("sectionTitle");

    auto *questionTitle = new QLabel("Question", preview);
    questionTitle->setObjectName("fieldTitle");
    m_questionPreviewLabel = new QLabel("Select a question to see full details.", preview);
    m_questionPreviewLabel->setWordWrap(true);
    m_questionPreviewLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto *answerTitle = new QLabel("Saved Answer", preview);
    answerTitle->setObjectName("fieldTitle");
    m_answerPreviewLabel = new QLabel("-", preview);
    m_answerPreviewLabel->setWordWrap(true);
    m_answerPreviewLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto *notesTitle = new QLabel("Notes", preview);
    notesTitle->setObjectName("fieldTitle");
    m_notesPreviewLabel = new QLabel("-", preview);
    m_notesPreviewLabel->setWordWrap(true);
    m_notesPreviewLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    previewLayout->addWidget(previewTitle);
    previewLayout->addSpacing(8);
    previewLayout->addWidget(questionTitle);
    previewLayout->addWidget(m_questionPreviewLabel);
    previewLayout->addSpacing(14);
    previewLayout->addWidget(answerTitle);
    previewLayout->addWidget(m_answerPreviewLabel);
    previewLayout->addSpacing(14);
    previewLayout->addWidget(notesTitle);
    previewLayout->addWidget(m_notesPreviewLabel);
    previewLayout->addStretch();

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_questionsTable);
    splitter->addWidget(preview);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 2);

    auto *addButton = new QPushButton("Add Question", this);
    auto *editButton = new QPushButton("Edit", this);
    auto *deleteButton = new QPushButton("Delete", this);
    auto *closeButton = new QPushButton("Close", this);

    connect(addButton, &QPushButton::clicked, this, &ApplicationDetailsDialog::addQuestion);
    connect(editButton, &QPushButton::clicked, this, &ApplicationDetailsDialog::editQuestion);
    connect(deleteButton, &QPushButton::clicked, this, &ApplicationDetailsDialog::deleteQuestion);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_questionsTable, &QTableWidget::itemSelectionChanged,
            this, &ApplicationDetailsDialog::updateQuestionPreview);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_infoLabel);
    layout->addWidget(splitter);
    layout->addLayout(buttonLayout);
    resize(980, 620);
}

void ApplicationDetailsDialog::loadQuestions()
{
    m_questions = m_questionRepository->findByApplication(m_application.id());
    m_questionsTable->setRowCount(m_questions.size());

    for (int row = 0; row < m_questions.size(); ++row) {
        const InterviewQuestion &question = m_questions[row];
        m_questionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(question.id())));
        m_questionsTable->setItem(row, 1, new QTableWidgetItem(question.question()));
        m_questionsTable->setItem(row, 2, new QTableWidgetItem(question.category()));
        m_questionsTable->setItem(row, 3, new QTableWidgetItem(question.difficulty()));
        m_questionsTable->setItem(row, 4, new QTableWidgetItem(question.practiced() ? "Yes" : "No"));
    }

    if (!m_questions.isEmpty()) {
        m_questionsTable->selectRow(0);
    } else {
        updateQuestionPreview();
    }
}

InterviewQuestion ApplicationDetailsDialog::selectedQuestion() const
{
    const int row = m_questionsTable->currentRow();
    if (row < 0 || row >= m_questions.size()) {
        return InterviewQuestion();
    }
    return m_questions[row];
}

int ApplicationDetailsDialog::selectedQuestionId() const
{
    return selectedQuestion().id();
}
