#ifndef JOBAPPLICATION_H
#define JOBAPPLICATION_H

#include <QString>

class JobApplication
{
public:
    JobApplication();
    JobApplication(int id,
                   const QString &company,
                   const QString &position,
                   const QString &jobUrl,
                   const QString &dateApplied,
                   const QString &status,
                   const QString &notes);

    int id() const;
    void setId(int id);

    QString company() const;
    void setCompany(const QString &company);

    QString position() const;
    void setPosition(const QString &position);

    QString jobUrl() const;
    void setJobUrl(const QString &jobUrl);

    QString dateApplied() const;
    void setDateApplied(const QString &dateApplied);

    QString status() const;
    void setStatus(const QString &status);

    QString notes() const;
    void setNotes(const QString &notes);

private:
    int m_id;
    QString m_company;
    QString m_position;
    QString m_jobUrl;
    QString m_dateApplied;
    QString m_status;
    QString m_notes;
};

#endif
