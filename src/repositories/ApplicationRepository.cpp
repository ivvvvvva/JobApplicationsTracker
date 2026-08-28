#include "repositories/ApplicationRepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

ApplicationRepository::ApplicationRepository(const QSqlDatabase &database)
    : m_database(database)
{
}

bool ApplicationRepository::add(JobApplication &application)
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO applications "
                  "(company, position, job_url, date_applied, status, notes) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(application.company());
    query.addBindValue(application.position());
    query.addBindValue(application.jobUrl());
    query.addBindValue(application.dateApplied());
    query.addBindValue(application.status());
    query.addBindValue(application.notes());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    application.setId(query.lastInsertId().toInt());
    return true;
}

bool ApplicationRepository::update(const JobApplication &application)
{
    QSqlQuery query(m_database);
    query.prepare("UPDATE applications SET "
                  "company = ?, position = ?, job_url = ?, date_applied = ?, status = ?, notes = ? "
                  "WHERE id = ?");
    query.addBindValue(application.company());
    query.addBindValue(application.position());
    query.addBindValue(application.jobUrl());
    query.addBindValue(application.dateApplied());
    query.addBindValue(application.status());
    query.addBindValue(application.notes());
    query.addBindValue(application.id());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool ApplicationRepository::remove(int id)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM applications WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

QList<JobApplication> ApplicationRepository::findAll(const QString &searchText,
                                                     const QString &status) const
{
    QList<JobApplication> applications;

    QString sql = "SELECT id, company, position, job_url, date_applied, status, notes "
                  "FROM applications WHERE 1 = 1";

    if (!searchText.trimmed().isEmpty()) {
        sql += " AND (company LIKE ? OR position LIKE ?)";
    }

    if (!status.trimmed().isEmpty() && status != "All") {
        sql += " AND status = ?";
    }

    sql += " ORDER BY date_applied DESC, company ASC";

    QSqlQuery query(m_database);
    query.prepare(sql);

    if (!searchText.trimmed().isEmpty()) {
        const QString pattern = "%" + searchText.trimmed() + "%";
        query.addBindValue(pattern);
        query.addBindValue(pattern);
    }

    if (!status.trimmed().isEmpty() && status != "All") {
        query.addBindValue(status);
    }

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return applications;
    }

    while (query.next()) {
        applications.append(readApplication(query));
    }

    return applications;
}

JobApplication ApplicationRepository::findById(int id) const
{
    QSqlQuery query(m_database);
    query.prepare("SELECT id, company, position, job_url, date_applied, status, notes "
                  "FROM applications WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return JobApplication();
    }

    if (query.next()) {
        return readApplication(query);
    }

    return JobApplication();
}

QMap<QString, int> ApplicationRepository::dashboardStats() const
{
    QMap<QString, int> stats;
    stats["total"] = 0;
    stats["active"] = 0;
    stats["interviews"] = 0;
    stats["offers"] = 0;
    stats["rejected"] = 0;

    QSqlQuery totalQuery("SELECT COUNT(*) FROM applications", m_database);
    if (totalQuery.next()) {
        stats["total"] = totalQuery.value(0).toInt();
    }

    QSqlQuery activeQuery("SELECT COUNT(*) FROM applications WHERE status NOT IN ('Rejected', 'Withdrawn')", m_database);
    if (activeQuery.next()) {
        stats["active"] = activeQuery.value(0).toInt();
    }

    QSqlQuery interviewQuery("SELECT COUNT(*) FROM applications "
                             "WHERE status IN ('HR Interview', 'Technical Interview', 'Final Interview')",
                             m_database);
    if (interviewQuery.next()) {
        stats["interviews"] = interviewQuery.value(0).toInt();
    }

    QSqlQuery offerQuery("SELECT COUNT(*) FROM applications WHERE status = 'Offer'", m_database);
    if (offerQuery.next()) {
        stats["offers"] = offerQuery.value(0).toInt();
    }

    QSqlQuery rejectedQuery("SELECT COUNT(*) FROM applications WHERE status = 'Rejected'", m_database);
    if (rejectedQuery.next()) {
        stats["rejected"] = rejectedQuery.value(0).toInt();
    }

    return stats;
}

QString ApplicationRepository::lastError() const
{
    return m_lastError;
}

JobApplication ApplicationRepository::readApplication(const QSqlQuery &query) const
{
    return JobApplication(query.value(0).toInt(),
                          query.value(1).toString(),
                          query.value(2).toString(),
                          query.value(3).toString(),
                          query.value(4).toString(),
                          query.value(5).toString(),
                          query.value(6).toString());
}
