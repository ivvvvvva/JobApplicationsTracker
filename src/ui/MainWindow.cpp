#include "ui/MainWindow.h"

#include "ui/ApplicationDetailsDialog.h"
#include "ui/ApplicationDialog.h"
#include "ui/MockInterviewDialog.h"

#include <QAbstractItemView>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(const QSqlDatabase &database, QWidget *parent)
    : QMainWindow(parent),
      m_applicationRepository(database),
      m_questionRepository(database)
{
    setupUi();
    refreshAll();
}

void MainWindow::refreshAll()
{
    refreshApplications();
    refreshQuestions();
    updateDashboard();
}

void MainWindow::addApplication()
{
    ApplicationDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    JobApplication application = dialog.application();
    if (!m_applicationRepository.add(application)) {
        QMessageBox::critical(this, "Database error", m_applicationRepository.lastError());
        return;
    }

    refreshAll();
}

void MainWindow::editApplication()
{
    JobApplication application = selectedApplication();
    if (application.id() < 0) {
        QMessageBox::information(this, "Select application", "Please select an application first.");
        return;
    }

    ApplicationDialog dialog(application, this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    if (!m_applicationRepository.update(dialog.application())) {
        QMessageBox::critical(this, "Database error", m_applicationRepository.lastError());
        return;
    }

    refreshAll();
}

void MainWindow::deleteApplication()
{
    JobApplication application = selectedApplication();
    if (application.id() < 0) {
        QMessageBox::information(this, "Select application", "Please select an application first.");
        return;
    }

    const QString message = QString("Delete application for %1 - %2?\nInterview questions will also be deleted.")
                                .arg(application.company(), application.position());
    if (QMessageBox::question(this, "Delete application", message) != QMessageBox::Yes) {
        return;
    }

    if (!m_applicationRepository.remove(application.id())) {
        QMessageBox::critical(this, "Database error", m_applicationRepository.lastError());
        return;
    }

    refreshAll();
}

void MainWindow::openApplicationDetails()
{
    JobApplication application = selectedApplication();
    if (application.id() < 0) {
        QMessageBox::information(this, "Select application", "Please select an application first.");
        return;
    }

    ApplicationDetailsDialog dialog(application, &m_questionRepository, this);
    dialog.exec();
    refreshAll();
}

void MainWindow::refreshApplications()
{
    m_applications = m_applicationRepository.findAll(m_applicationSearchEdit->text(),
                                                     m_statusFilterCombo->currentText());
    m_applicationsTable->setRowCount(m_applications.size());

    for (int row = 0; row < m_applications.size(); ++row) {
        const JobApplication &application = m_applications[row];
        m_applicationsTable->setItem(row, 0, new QTableWidgetItem(QString::number(application.id())));
        m_applicationsTable->setItem(row, 1, new QTableWidgetItem(application.company()));
        m_applicationsTable->setItem(row, 2, new QTableWidgetItem(application.position()));
        m_applicationsTable->setItem(row, 3, new QTableWidgetItem(application.dateApplied()));
        m_applicationsTable->setItem(row, 4, new QTableWidgetItem(application.status()));
        m_applicationsTable->setItem(row, 5, new QTableWidgetItem(application.jobUrl()));
    }
}

void MainWindow::refreshQuestions()
{
    m_questions = m_questionRepository.findAll(m_questionSearchEdit->text(),
                                               m_categoryFilterCombo->currentText(),
                                               m_difficultyFilterCombo->currentText());
    m_questionsTable->setRowCount(m_questions.size());

    for (int row = 0; row < m_questions.size(); ++row) {
        const InterviewQuestion &question = m_questions[row];
        m_questionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(question.id())));
        m_questionsTable->setItem(row, 1, new QTableWidgetItem(question.question()));
        m_questionsTable->setItem(row, 2, new QTableWidgetItem(question.category()));
        m_questionsTable->setItem(row, 3, new QTableWidgetItem(question.difficulty()));
        m_questionsTable->setItem(row, 4, new QTableWidgetItem(question.practiced() ? "Yes" : "No"));
        m_questionsTable->setItem(row, 5, new QTableWidgetItem(question.myAnswer()));
    }
}

void MainWindow::togglePracticed()
{
    InterviewQuestion question = selectedQuestion();
    if (question.id() < 0) {
        QMessageBox::information(this, "Select question", "Please select a question first.");
        return;
    }

    if (!m_questionRepository.setPracticed(question.id(), !question.practiced())) {
        QMessageBox::critical(this, "Database error", m_questionRepository.lastError());
        return;
    }

    refreshAll();
}

void MainWindow::openMockInterview()
{
    MockInterviewDialog dialog(&m_questionRepository, this);
    dialog.exec();
}

void MainWindow::setupUi()
{
    setWindowTitle("Job Applications Tracker");

    auto *tabs = new QTabWidget(this);
    tabs->addTab(createDashboardTab(), "Dashboard");
    tabs->addTab(createApplicationsTab(), "Applications");
    tabs->addTab(createQuestionBankTab(), "Question Bank");
    tabs->addTab(createMockInterviewTab(), "Mock Interview");

    setCentralWidget(tabs);
    resize(1050, 680);
}

QWidget *MainWindow::createDashboardTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QGridLayout(tab);

    auto createBox = [this](const QString &title, QLabel **valueLabel) {
        auto *box = new QGroupBox(title, this);
        auto *boxLayout = new QVBoxLayout(box);
        *valueLabel = new QLabel("0", box);
        QFont font = (*valueLabel)->font();
        font.setPointSize(24);
        font.setBold(true);
        (*valueLabel)->setFont(font);
        (*valueLabel)->setAlignment(Qt::AlignCenter);
        boxLayout->addWidget(*valueLabel);
        return box;
    };

    layout->addWidget(createBox("Total Applications", &m_totalApplicationsLabel), 0, 0);
    layout->addWidget(createBox("Active Applications", &m_activeApplicationsLabel), 0, 1);
    layout->addWidget(createBox("Interviews", &m_interviewsLabel), 0, 2);
    layout->addWidget(createBox("Offers", &m_offersLabel), 1, 0);
    layout->addWidget(createBox("Rejected", &m_rejectedLabel), 1, 1);
    layout->addWidget(createBox("Interview Questions", &m_questionsCountLabel), 1, 2);
    layout->setRowStretch(2, 1);

    return tab;
}

QWidget *MainWindow::createApplicationsTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);

    m_applicationSearchEdit = new QLineEdit(this);
    m_applicationSearchEdit->setPlaceholderText("Search company or position");

    m_statusFilterCombo = new QComboBox(this);
    m_statusFilterCombo->addItems({"All", "Applied", "HR Interview", "Technical Interview",
                                   "Final Interview", "Offer", "Rejected", "Withdrawn"});

    auto *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(m_applicationSearchEdit);
    filterLayout->addWidget(m_statusFilterCombo);

    auto *addButton = new QPushButton("Add", this);
    auto *editButton = new QPushButton("Edit", this);
    auto *deleteButton = new QPushButton("Delete", this);
    auto *detailsButton = new QPushButton("Details / Questions", this);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(detailsButton);
    buttonLayout->addStretch();

    m_applicationsTable = new QTableWidget(this);
    m_applicationsTable->setColumnCount(6);
    m_applicationsTable->setHorizontalHeaderLabels({"ID", "Company", "Position", "Date Applied", "Status", "Job URL"});
    m_applicationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_applicationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_applicationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_applicationsTable->hideColumn(0);
    m_applicationsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_applicationsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_applicationsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    connect(m_applicationSearchEdit, &QLineEdit::textChanged, this, &MainWindow::refreshApplications);
    connect(m_statusFilterCombo, &QComboBox::currentTextChanged, this, &MainWindow::refreshApplications);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addApplication);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editApplication);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteApplication);
    connect(detailsButton, &QPushButton::clicked, this, &MainWindow::openApplicationDetails);
    connect(m_applicationsTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::openApplicationDetails);

    layout->addLayout(filterLayout);
    layout->addLayout(buttonLayout);
    layout->addWidget(m_applicationsTable);
    return tab;
}

QWidget *MainWindow::createQuestionBankTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);

    m_questionSearchEdit = new QLineEdit(this);
    m_questionSearchEdit->setPlaceholderText("Search questions, answers or notes");

    m_categoryFilterCombo = new QComboBox(this);
    m_categoryFilterCombo->addItems({"All", "Behavioral", "C++", "SQL", "Algorithms", "Data Science", "Other"});

    m_difficultyFilterCombo = new QComboBox(this);
    m_difficultyFilterCombo->addItems({"All", "Easy", "Medium", "Hard"});

    auto *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(m_questionSearchEdit);
    filterLayout->addWidget(m_categoryFilterCombo);
    filterLayout->addWidget(m_difficultyFilterCombo);

    auto *toggleButton = new QPushButton("Toggle Practiced", this);
    auto *refreshButton = new QPushButton("Refresh", this);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(toggleButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();

    m_questionsTable = new QTableWidget(this);
    m_questionsTable->setColumnCount(6);
    m_questionsTable->setHorizontalHeaderLabels({"ID", "Question", "Category", "Difficulty", "Practiced", "My Answer"});
    m_questionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_questionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_questionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_questionsTable->hideColumn(0);
    m_questionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_questionsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    connect(m_questionSearchEdit, &QLineEdit::textChanged, this, &MainWindow::refreshQuestions);
    connect(m_categoryFilterCombo, &QComboBox::currentTextChanged, this, &MainWindow::refreshQuestions);
    connect(m_difficultyFilterCombo, &QComboBox::currentTextChanged, this, &MainWindow::refreshQuestions);
    connect(toggleButton, &QPushButton::clicked, this, &MainWindow::togglePracticed);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshAll);

    layout->addLayout(filterLayout);
    layout->addLayout(buttonLayout);
    layout->addWidget(m_questionsTable);
    return tab;
}

QWidget *MainWindow::createMockInterviewTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    auto *button = new QPushButton("Start Mock Interview", this);
    button->setMinimumHeight(48);

    auto *label = new QLabel("Start a simple practice session with random saved interview questions.", this);
    label->setAlignment(Qt::AlignCenter);

    connect(button, &QPushButton::clicked, this, &MainWindow::openMockInterview);

    layout->addStretch();
    layout->addWidget(label);
    layout->addWidget(button);
    layout->addStretch();
    return tab;
}

void MainWindow::updateDashboard()
{
    const QMap<QString, int> stats = m_applicationRepository.dashboardStats();
    m_totalApplicationsLabel->setText(QString::number(stats.value("total")));
    m_activeApplicationsLabel->setText(QString::number(stats.value("active")));
    m_interviewsLabel->setText(QString::number(stats.value("interviews")));
    m_offersLabel->setText(QString::number(stats.value("offers")));
    m_rejectedLabel->setText(QString::number(stats.value("rejected")));
    m_questionsCountLabel->setText(QString::number(m_questionRepository.countAll()));
}

JobApplication MainWindow::selectedApplication() const
{
    const int row = m_applicationsTable->currentRow();
    if (row < 0 || row >= m_applications.size()) {
        return JobApplication();
    }
    return m_applications[row];
}

InterviewQuestion MainWindow::selectedQuestion() const
{
    const int row = m_questionsTable->currentRow();
    if (row < 0 || row >= m_questions.size()) {
        return InterviewQuestion();
    }
    return m_questions[row];
}
