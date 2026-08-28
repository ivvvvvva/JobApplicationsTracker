#ifndef APPLICATIONDIALOG_H
#define APPLICATIONDIALOG_H

#include "models/JobApplication.h"

#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>

class ApplicationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicationDialog(QWidget *parent = nullptr);
    explicit ApplicationDialog(const JobApplication &application, QWidget *parent = nullptr);

    JobApplication application() const;

private:
    void setupUi();
    void loadApplication(const JobApplication &application);

    int m_id;
    QLineEdit *m_companyEdit;
    QLineEdit *m_positionEdit;
    QLineEdit *m_urlEdit;
    QDateEdit *m_dateEdit;
    QComboBox *m_statusCombo;
    QTextEdit *m_notesEdit;
};

#endif
