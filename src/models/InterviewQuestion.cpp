#include "models/InterviewQuestion.h"

InterviewQuestion::InterviewQuestion()
    : m_id(-1),
      m_applicationId(-1),
      m_practiced(false)
{
}

InterviewQuestion::InterviewQuestion(int id,
                                     int applicationId,
                                     const QString &question,
                                     const QString &category,
                                     const QString &difficulty,
                                     const QString &myAnswer,
                                     const QString &notes,
                                     bool practiced)
    : m_id(id),
      m_applicationId(applicationId),
      m_question(question),
      m_category(category),
      m_difficulty(difficulty),
      m_myAnswer(myAnswer),
      m_notes(notes),
      m_practiced(practiced)
{
}

int InterviewQuestion::id() const { return m_id; }
void InterviewQuestion::setId(int id) { m_id = id; }

int InterviewQuestion::applicationId() const { return m_applicationId; }
void InterviewQuestion::setApplicationId(int applicationId) { m_applicationId = applicationId; }

QString InterviewQuestion::question() const { return m_question; }
void InterviewQuestion::setQuestion(const QString &question) { m_question = question; }

QString InterviewQuestion::category() const { return m_category; }
void InterviewQuestion::setCategory(const QString &category) { m_category = category; }

QString InterviewQuestion::difficulty() const { return m_difficulty; }
void InterviewQuestion::setDifficulty(const QString &difficulty) { m_difficulty = difficulty; }

QString InterviewQuestion::myAnswer() const { return m_myAnswer; }
void InterviewQuestion::setMyAnswer(const QString &myAnswer) { m_myAnswer = myAnswer; }

QString InterviewQuestion::notes() const { return m_notes; }
void InterviewQuestion::setNotes(const QString &notes) { m_notes = notes; }

bool InterviewQuestion::practiced() const { return m_practiced; }
void InterviewQuestion::setPracticed(bool practiced) { m_practiced = practiced; }
