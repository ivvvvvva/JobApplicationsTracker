#include "models/JobApplication.h"

JobApplication::JobApplication()
    : m_id(-1)
{
}

JobApplication::JobApplication(int id,
                               const QString &company,
                               const QString &position,
                               const QString &jobUrl,
                               const QString &dateApplied,
                               const QString &status,
                               const QString &notes)
    : m_id(id),
      m_company(company),
      m_position(position),
      m_jobUrl(jobUrl),
      m_dateApplied(dateApplied),
      m_status(status),
      m_notes(notes)
{
}

int JobApplication::id() const { return m_id; }
void JobApplication::setId(int id) { m_id = id; }

QString JobApplication::company() const { return m_company; }
void JobApplication::setCompany(const QString &company) { m_company = company; }

QString JobApplication::position() const { return m_position; }
void JobApplication::setPosition(const QString &position) { m_position = position; }

QString JobApplication::jobUrl() const { return m_jobUrl; }
void JobApplication::setJobUrl(const QString &jobUrl) { m_jobUrl = jobUrl; }

QString JobApplication::dateApplied() const { return m_dateApplied; }
void JobApplication::setDateApplied(const QString &dateApplied) { m_dateApplied = dateApplied; }

QString JobApplication::status() const { return m_status; }
void JobApplication::setStatus(const QString &status) { m_status = status; }

QString JobApplication::notes() const { return m_notes; }
void JobApplication::setNotes(const QString &notes) { m_notes = notes; }
