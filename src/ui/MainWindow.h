#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "models/InterviewQuestion.h"
#include "models/JobApplication.h"
#include "repositories/ApplicationRepository.h"
#include "repositories/QuestionRepository.h"

#include <QComboBox>
#include <QLabel>
#include <QList>
#include <QLineEdit>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QSqlDatabase &database, QWidget *parent = nullptr);

private slots:
    void refreshAll();
    void addApplication();
    void editApplication();
    void deleteApplication();
    void openApplicationDetails();
    void refreshApplications();
    void refreshQuestions();
    void togglePracticed();
    void openMockInterview();

private:
    void setupUi();
    QWidget *createDashboardTab();
    QWidget *createApplicationsTab();
    QWidget *createQuestionBankTab();
    QWidget *createMockInterviewTab();
    void updateDashboard();
    JobApplication selectedApplication() const;
    InterviewQuestion selectedQuestion() const;

    ApplicationRepository m_applicationRepository;
    QuestionRepository m_questionRepository;

    QLabel *m_totalApplicationsLabel;
    QLabel *m_activeApplicationsLabel;
    QLabel *m_interviewsLabel;
    QLabel *m_offersLabel;
    QLabel *m_rejectedLabel;
    QLabel *m_questionsCountLabel;

    QLineEdit *m_applicationSearchEdit;
    QComboBox *m_statusFilterCombo;
    QTableWidget *m_applicationsTable;
    QList<JobApplication> m_applications;

    QLineEdit *m_questionSearchEdit;
    QComboBox *m_categoryFilterCombo;
    QComboBox *m_difficultyFilterCombo;
    QTableWidget *m_questionsTable;
    QList<InterviewQuestion> m_questions;
};

#endif
