#ifndef APPLICATIONREPOSITORY_H
#define APPLICATIONREPOSITORY_H

#include "models/JobApplication.h"

#include <QList>
#include <QMap>
#include <QSqlDatabase>
#include <QString>

class QSqlQuery;

class ApplicationRepository
{
public:
    explicit ApplicationRepository(const QSqlDatabase &database);

    bool add(JobApplication &application);
    bool update(const JobApplication &application);
    bool remove(int id);

    QList<JobApplication> findAll(const QString &searchText = QString(),
                                  const QString &status = QString()) const;
    JobApplication findById(int id) const;
    QMap<QString, int> dashboardStats() const;
    QString lastError() const;

private:
    JobApplication readApplication(const QSqlQuery &query) const;

    QSqlDatabase m_database;
    mutable QString m_lastError;
};

#endif
