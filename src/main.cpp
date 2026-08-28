#include "database/DatabaseManager.h"
#include "ui/MainWindow.h"

#include <QApplication>
#include <QMessageBox>

static QString applicationStyleSheet()
{
    return R"(
        QWidget {
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 10pt;
            color: #1f2937;
            background: #f4f6f8;
        }

        QMainWindow, QDialog {
            background: #f4f6f8;
        }

        QTabWidget::pane {
            border: 1px solid #d7dde5;
            background: #ffffff;
            border-radius: 6px;
        }

        QTabBar::tab {
            background: #e8edf3;
            padding: 9px 16px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background: #ffffff;
            color: #0f766e;
            font-weight: 600;
        }

        QGroupBox, QLabel#infoPanel, QWidget#previewPanel {
            background: #ffffff;
            border: 1px solid #d7dde5;
            border-radius: 8px;
            padding: 12px;
        }

        QLabel#sectionTitle {
            font-size: 14pt;
            font-weight: 700;
            color: #111827;
            border: none;
            padding: 0;
            background: transparent;
        }

        QLabel#fieldTitle {
            font-weight: 700;
            color: #0f766e;
            border: none;
            padding: 0;
            background: transparent;
        }

        QLineEdit, QTextEdit, QComboBox, QDateEdit, QSpinBox {
            background: #ffffff;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            padding: 7px;
            selection-background-color: #0f766e;
        }

        QPushButton {
            background: #0f766e;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 8px 14px;
            font-weight: 600;
        }

        QPushButton:hover {
            background: #115e59;
        }

        QPushButton:disabled {
            background: #a7b4c2;
        }

        QTableWidget {
            background: #ffffff;
            alternate-background-color: #f8fafc;
            border: 1px solid #d7dde5;
            border-radius: 8px;
            gridline-color: #e5e7eb;
        }

        QHeaderView::section {
            background: #eef2f7;
            color: #111827;
            border: none;
            border-right: 1px solid #d7dde5;
            padding: 8px;
            font-weight: 700;
        }

        QTableWidget::item {
            padding: 7px;
        }

        QTableWidget::item:selected {
            background: #ccfbf1;
            color: #0f172a;
        }
    )";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(applicationStyleSheet());

    DatabaseManager databaseManager;
    if (!databaseManager.open() || !databaseManager.initialize()) {
        QMessageBox::critical(nullptr,
                              "Database error",
                              "Could not initialize database:\n" + databaseManager.lastError());
        return 1;
    }

    MainWindow window(databaseManager.database());
    window.show();

    return app.exec();
}
