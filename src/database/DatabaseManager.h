#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    explicit DatabaseManager(const QString &databasePath = "job_applications.db");

    bool open();
    bool initialize();
    QSqlDatabase database() const;
    QString lastError() const;

private:
    bool executeSchemaFile(const QString &schemaPath);

    QString m_databasePath;
    QSqlDatabase m_database;
    QString m_lastError;
};

#endif
