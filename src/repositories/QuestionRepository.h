#ifndef QUESTIONREPOSITORY_H
#define QUESTIONREPOSITORY_H

#include "models/InterviewQuestion.h"

#include <QList>
#include <QSqlDatabase>
#include <QString>

class QSqlQuery;

class QuestionRepository
{
public:
    explicit QuestionRepository(const QSqlDatabase &database);

    bool add(InterviewQuestion &question);
    bool update(const InterviewQuestion &question);
    bool remove(int id);
    bool setPracticed(int id, bool practiced);

    QList<InterviewQuestion> findByApplication(int applicationId) const;
    QList<InterviewQuestion> findAll(const QString &searchText = QString(),
                                     const QString &category = QString(),
                                     const QString &difficulty = QString()) const;
    QList<InterviewQuestion> randomQuestions(const QString &category, int count) const;
    int countAll() const;
    QString lastError() const;

private:
    InterviewQuestion readQuestion(const QSqlQuery &query) const;

    QSqlDatabase m_database;
    mutable QString m_lastError;
};

#endif
