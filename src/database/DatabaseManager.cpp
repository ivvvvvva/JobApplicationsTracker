#include "database/DatabaseManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>

DatabaseManager::DatabaseManager(const QString &databasePath)
    : m_databasePath(databasePath)
{
}

bool DatabaseManager::open()
{
    if (QSqlDatabase::contains("main_connection")) {
        m_database = QSqlDatabase::database("main_connection");
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE", "main_connection");
        m_database.setDatabaseName(m_databasePath);
    }

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        return false;
    }

    QSqlQuery query(m_database);
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        m_lastError = query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::initialize()
{
    if (!m_database.isOpen() && !open()) {
        return false;
    }

    const QString appDirSchema = QCoreApplication::applicationDirPath() + "/database/schema.sql";
    if (QFile::exists(appDirSchema)) {
        return executeSchemaFile(appDirSchema);
    }

    const QString sourceDirSchema = QDir::currentPath() + "/database/schema.sql";
    return executeSchemaFile(sourceDirSchema);
}

QSqlDatabase DatabaseManager::database() const
{
    return m_database;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

bool DatabaseManager::executeSchemaFile(const QString &schemaPath)
{
    QFile file(schemaPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = "Cannot open schema file: " + schemaPath;
        return false;
    }

    QTextStream stream(&file);
    const QString schema = stream.readAll();
    const QStringList statements = schema.split(';');

    for (const QString &statement : statements) {
        const QString trimmed = statement.trimmed();
        if (trimmed.isEmpty()) {
            continue;
        }

        QSqlQuery query(m_database);
        if (!query.exec(trimmed)) {
            m_lastError = query.lastError().text();
            return false;
        }
    }

    return true;
}
