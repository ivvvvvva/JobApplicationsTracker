#ifndef INTERVIEWQUESTION_H
#define INTERVIEWQUESTION_H

#include <QString>

class InterviewQuestion
{
public:
    InterviewQuestion();
    InterviewQuestion(int id,
                      int applicationId,
                      const QString &question,
                      const QString &category,
                      const QString &difficulty,
                      const QString &myAnswer,
                      const QString &notes,
                      bool practiced);

    int id() const;
    void setId(int id);

    int applicationId() const;
    void setApplicationId(int applicationId);

    QString question() const;
    void setQuestion(const QString &question);

    QString category() const;
    void setCategory(const QString &category);

    QString difficulty() const;
    void setDifficulty(const QString &difficulty);

    QString myAnswer() const;
    void setMyAnswer(const QString &myAnswer);

    QString notes() const;
    void setNotes(const QString &notes);

    bool practiced() const;
    void setPracticed(bool practiced);

private:
    int m_id;
    int m_applicationId;
    QString m_question;
    QString m_category;
    QString m_difficulty;
    QString m_myAnswer;
    QString m_notes;
    bool m_practiced;
};

#endif
