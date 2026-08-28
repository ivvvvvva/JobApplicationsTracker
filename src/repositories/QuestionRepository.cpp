#include "repositories/QuestionRepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

QuestionRepository::QuestionRepository(const QSqlDatabase &database)
    : m_database(database)
{
}

bool QuestionRepository::add(InterviewQuestion &question)
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO questions "
                  "(application_id, question, category, difficulty, my_answer, notes, practiced) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(question.applicationId());
    query.addBindValue(question.question());
    query.addBindValue(question.category());
    query.addBindValue(question.difficulty());
    query.addBindValue(question.myAnswer());
    query.addBindValue(question.notes());
    query.addBindValue(question.practiced() ? 1 : 0);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    question.setId(query.lastInsertId().toInt());
    return true;
}

bool QuestionRepository::update(const InterviewQuestion &question)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE questions SET "
                  "application_id = ?, question = ?, category = ?, difficulty = ?, "
                  "my_answer = ?, notes = ?, practiced = ? WHERE id = ?");
    query.addBindValue(question.applicationId());
    query.addBindValue(question.question());
    query.addBindValue(question.category());
    query.addBindValue(question.difficulty());
    query.addBindValue(question.myAnswer());
    query.addBindValue(question.notes());
    query.addBindValue(question.practiced() ? 1 : 0);
    query.addBindValue(question.id());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool QuestionRepository::remove(int id)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM questions WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool QuestionRepository::setPracticed(int id, bool practiced)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE questions SET practiced = ? WHERE id = ?");
    query.addBindValue(practiced ? 1 : 0);
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

QList<InterviewQuestion> QuestionRepository::findByApplication(int applicationId) const
{
    QList<InterviewQuestion> questions;
    QSqlQuery query(m_database);
    query.prepare("SELECT id, application_id, question, category, difficulty, my_answer, notes, practiced "
                  "FROM questions WHERE application_id = ? ORDER BY id DESC");
    query.addBindValue(applicationId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return questions;
    }

    while (query.next()) {
        questions.append(readQuestion(query));
    }

    return questions;
}

QList<InterviewQuestion> QuestionRepository::findAll(const QString &searchText,
                                                     const QString &category,
                                                     const QString &difficulty) const
{
    QList<InterviewQuestion> questions;
    QString sql = "SELECT id, application_id, question, category, difficulty, my_answer, notes, practiced "
                  "FROM questions WHERE 1 = 1";

    if (!searchText.trimmed().isEmpty()) {
        sql += " AND (question LIKE ? OR my_answer LIKE ? OR notes LIKE ?)";
    }
    if (!category.trimmed().isEmpty() && category != "All") {
        sql += " AND category = ?";
    }
    if (!difficulty.trimmed().isEmpty() && difficulty != "All") {
        sql += " AND difficulty = ?";
    }

    sql += " ORDER BY category ASC, difficulty ASC, id DESC";

    QSqlQuery query(m_database);
    query.prepare(sql);

    if (!searchText.trimmed().isEmpty()) {
        const QString pattern = "%" + searchText.trimmed() + "%";
        query.addBindValue(pattern);
        query.addBindValue(pattern);
        query.addBindValue(pattern);
    }
    if (!category.trimmed().isEmpty() && category != "All") {
        query.addBindValue(category);
    }
    if (!difficulty.trimmed().isEmpty() && difficulty != "All") {
        query.addBindValue(difficulty);
    }

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return questions;
    }

    while (query.next()) {
        questions.append(readQuestion(query));
    }

    return questions;
}

QList<InterviewQuestion> QuestionRepository::randomQuestions(const QString &category, int count) const
{
    QList<InterviewQuestion> questions;
    QString sql = "SELECT id, application_id, question, category, difficulty, my_answer, notes, practiced "
                  "FROM questions";

    if (!category.trimmed().isEmpty() && category != "All") {
        sql += " WHERE category = ?";
    }

    sql += " ORDER BY RANDOM() LIMIT ?";

    QSqlQuery query(m_database);
    query.prepare(sql);

    if (!category.trimmed().isEmpty() && category != "All") {
        query.addBindValue(category);
    }
    query.addBindValue(count);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return questions;
    }

    while (query.next()) {
        questions.append(readQuestion(query));
    }

    return questions;
}

int QuestionRepository::countAll() const
{
    QSqlQuery query("SELECT COUNT(*) FROM questions", m_database);
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

QString QuestionRepository::lastError() const
{
    return m_lastError;
}

InterviewQuestion QuestionRepository::readQuestion(const QSqlQuery &query) const
{
    return InterviewQuestion(query.value(0).toInt(),
                             query.value(1).toInt(),
                             query.value(2).toString(),
                             query.value(3).toString(),
                             query.value(4).toString(),
                             query.value(5).toString(),
                             query.value(6).toString(),
                             query.value(7).toInt() == 1);
}
