#include "ui/ApplicationDialog.h"

#include <QDate>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QVBoxLayout>

ApplicationDialog::ApplicationDialog(QWidget *parent)
    : QDialog(parent),
      m_id(-1)
{
    setupUi();
    setWindowTitle("Add Application");
}

ApplicationDialog::ApplicationDialog(const JobApplication &application, QWidget *parent)
    : QDialog(parent),
      m_id(application.id())
{
    setupUi();
    setWindowTitle("Edit Application");
    loadApplication(application);
}

JobApplication ApplicationDialog::application() const
{
    return JobApplication(m_id,
                          m_companyEdit->text().trimmed(),
                          m_positionEdit->text().trimmed(),
                          m_urlEdit->text().trimmed(),
                          m_dateEdit->date().toString(Qt::ISODate),
                          m_statusCombo->currentText(),
                          m_notesEdit->toPlainText().trimmed());
}

void ApplicationDialog::setupUi()
{
    m_companyEdit = new QLineEdit(this);
    m_positionEdit = new QLineEdit(this);
    m_urlEdit = new QLineEdit(this);
    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("yyyy-MM-dd");

    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems({"Applied", "HR Interview", "Technical Interview", "Final Interview",
                             "Offer", "Rejected", "Withdrawn"});

    m_notesEdit = new QTextEdit(this);
    m_notesEdit->setMinimumHeight(90);

    auto *form = new QFormLayout;
    form->addRow("Company:", m_companyEdit);
    form->addRow("Position:", m_positionEdit);
    form->addRow("Job URL:", m_urlEdit);
    form->addRow("Date Applied:", m_dateEdit);
    form->addRow("Status:", m_statusCombo);
    form->addRow("Notes:", m_notesEdit);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (m_companyEdit->text().trimmed().isEmpty() || m_positionEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Missing data", "Company and position are required.");
            return;
        }
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
    resize(460, 360);
}

void ApplicationDialog::loadApplication(const JobApplication &application)
{
    m_companyEdit->setText(application.company());
    m_positionEdit->setText(application.position());
    m_urlEdit->setText(application.jobUrl());
    m_dateEdit->setDate(QDate::fromString(application.dateApplied(), Qt::ISODate));
    m_statusCombo->setCurrentText(application.status());
    m_notesEdit->setPlainText(application.notes());
}
