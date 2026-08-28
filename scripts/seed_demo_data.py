import sqlite3
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DB_PATH = ROOT / "job_applications.db"


APPLICATIONS = [
    {
        "company": "Nordeus",
        "position": "Junior Data Scientist",
        "job_url": "https://example.com/nordeus-junior-data-scientist",
        "date_applied": "2026-08-01",
        "status": "Technical Interview",
        "notes": "Focus on product analytics, SQL, A/B testing and Python basics.",
        "questions": [
            {
                "question": "How would you measure whether a new game feature improves player retention?",
                "category": "Data Science",
                "difficulty": "Medium",
                "my_answer": "Define retention metrics such as D1, D7 and D30 retention, split users into control and treatment groups, run an A/B test, and compare statistically significant changes while checking sample size and segment effects.",
                "notes": "Mention guardrail metrics like revenue, session length and crash rate.",
                "practiced": 1,
            },
            {
                "question": "Explain the difference between precision and recall.",
                "category": "Data Science",
                "difficulty": "Easy",
                "my_answer": "Precision measures how many predicted positives are actually positive. Recall measures how many real positives the model successfully found.",
                "notes": "Good example: fraud detection or spam classification.",
                "practiced": 1,
            },
        ],
    },
    {
        "company": "Seven Bridges",
        "position": "Data Engineer Intern",
        "job_url": "https://example.com/seven-bridges-data-engineer-intern",
        "date_applied": "2026-08-04",
        "status": "HR Interview",
        "notes": "Healthcare/genomics company. Prepare SQL joins, pipelines and data quality examples.",
        "questions": [
            {
                "question": "What is the difference between INNER JOIN and LEFT JOIN?",
                "category": "SQL",
                "difficulty": "Easy",
                "my_answer": "INNER JOIN returns only matching rows from both tables. LEFT JOIN returns all rows from the left table and matching rows from the right table, with NULL where there is no match.",
                "notes": "Draw two small tables if explaining at defense.",
                "practiced": 1,
            },
            {
                "question": "How would you design a daily ETL pipeline for processing application events?",
                "category": "Data Science",
                "difficulty": "Medium",
                "my_answer": "Ingest raw events, validate schema, remove duplicates, transform into analytical tables, store failed records separately, log pipeline runs and monitor row counts and freshness.",
                "notes": "Although category list has no Data Engineering option, this belongs closest to Data Science/SQL.",
                "practiced": 0,
            },
        ],
    },
    {
        "company": "Microsoft",
        "position": "Data Analyst Intern",
        "job_url": "https://example.com/microsoft-data-analyst-intern",
        "date_applied": "2026-08-07",
        "status": "Applied",
        "notes": "Prepare business metrics, dashboards and SQL aggregation questions.",
        "questions": [
            {
                "question": "Write a query that returns the number of applications per status.",
                "category": "SQL",
                "difficulty": "Easy",
                "my_answer": "SELECT status, COUNT(*) FROM applications GROUP BY status;",
                "notes": "Useful because it connects directly to this project.",
                "practiced": 0,
            },
            {
                "question": "How would you explain a dashboard metric that suddenly dropped by 30 percent?",
                "category": "Behavioral",
                "difficulty": "Medium",
                "my_answer": "First I would validate the data pipeline and metric definition, then segment by date, platform, geography and user group. After that I would compare with releases, incidents or marketing changes.",
                "notes": "Shows structured thinking and communication.",
                "practiced": 0,
            },
        ],
    },
    {
        "company": "Databricks",
        "position": "Junior Data Engineer",
        "job_url": "https://example.com/databricks-junior-data-engineer",
        "date_applied": "2026-08-10",
        "status": "Final Interview",
        "notes": "Prepare distributed data concepts, SQL, Python and pipeline reliability.",
        "questions": [
            {
                "question": "What is partitioning in large data tables and why is it useful?",
                "category": "Data Science",
                "difficulty": "Medium",
                "my_answer": "Partitioning stores data by values such as date or region so queries can scan only relevant partitions. It improves performance and reduces cost when filters match partition columns.",
                "notes": "Mention avoiding too many tiny partitions.",
                "practiced": 1,
            },
            {
                "question": "How do you handle duplicate records in a data pipeline?",
                "category": "SQL",
                "difficulty": "Medium",
                "my_answer": "Use a stable unique key if available, otherwise define matching columns, rank rows with ROW_NUMBER over a partition, keep the newest or most reliable row and log removed duplicates.",
                "notes": "Could mention idempotent pipeline design.",
                "practiced": 0,
            },
        ],
    },
    {
        "company": "Levi9",
        "position": "Junior C++ / Data Tools Developer",
        "job_url": "https://example.com/levi9-cpp-data-tools",
        "date_applied": "2026-08-13",
        "status": "Rejected",
        "notes": "Good practice for C++ basics and simple desktop tooling.",
        "questions": [
            {
                "question": "What is RAII in C++?",
                "category": "C++",
                "difficulty": "Medium",
                "my_answer": "RAII means resource acquisition is initialization. A resource is tied to an object's lifetime, so constructors acquire resources and destructors release them automatically.",
                "notes": "Examples: smart pointers, file handles, database connection wrapper.",
                "practiced": 1,
            },
            {
                "question": "Why should SQL queries use prepared statements?",
                "category": "SQL",
                "difficulty": "Medium",
                "my_answer": "Prepared statements separate SQL structure from user input, reducing SQL injection risk and making repeated queries cleaner and sometimes faster.",
                "notes": "This project uses prepared statements in repositories.",
                "practiced": 1,
            },
        ],
    },
    {
        "company": "Yettel",
        "position": "Data Science Intern",
        "job_url": "https://example.com/yettel-data-science-intern",
        "date_applied": "2026-08-18",
        "status": "Offer",
        "notes": "Telecom churn prediction and customer segmentation topics.",
        "questions": [
            {
                "question": "How would you approach customer churn prediction?",
                "category": "Data Science",
                "difficulty": "Hard",
                "my_answer": "Define churn clearly, collect historical customer behavior, build features such as usage trends and support tickets, train a classification model, evaluate with recall/precision or ROC-AUC, and explain drivers to business stakeholders.",
                "notes": "Mention leakage: avoid using future information.",
                "practiced": 0,
            },
            {
                "question": "What is data leakage in machine learning?",
                "category": "Data Science",
                "difficulty": "Medium",
                "my_answer": "Data leakage happens when training data contains information that would not be available at prediction time, causing overly optimistic evaluation and poor real-world performance.",
                "notes": "Useful example: cancellation date in churn model.",
                "practiced": 0,
            },
        ],
    },
]


def ensure_schema(connection):
    schema_path = ROOT / "database" / "schema.sql"
    connection.executescript(schema_path.read_text(encoding="utf-8"))


def clear_demo_rows(connection):
    companies = [application["company"] for application in APPLICATIONS]
    placeholders = ",".join("?" for _ in companies)
    connection.execute(
        f"DELETE FROM applications WHERE company IN ({placeholders})",
        companies,
    )


def insert_demo_rows(connection):
    for application in APPLICATIONS:
        cursor = connection.execute(
            """
            INSERT INTO applications
                (company, position, job_url, date_applied, status, notes)
            VALUES (?, ?, ?, ?, ?, ?)
            """,
            (
                application["company"],
                application["position"],
                application["job_url"],
                application["date_applied"],
                application["status"],
                application["notes"],
            ),
        )
        application_id = cursor.lastrowid

        for question in application["questions"]:
            connection.execute(
                """
                INSERT INTO questions
                    (application_id, question, category, difficulty, my_answer, notes, practiced)
                VALUES (?, ?, ?, ?, ?, ?, ?)
                """,
                (
                    application_id,
                    question["question"],
                    question["category"],
                    question["difficulty"],
                    question["my_answer"],
                    question["notes"],
                    question["practiced"],
                ),
            )


def main():
    with sqlite3.connect(DB_PATH) as connection:
        connection.execute("PRAGMA foreign_keys = ON")
        ensure_schema(connection)
        clear_demo_rows(connection)
        insert_demo_rows(connection)

    print(f"Demo data inserted into {DB_PATH}")
    print(f"Applications: {len(APPLICATIONS)}")
    print(f"Questions: {sum(len(application['questions']) for application in APPLICATIONS)}")


if __name__ == "__main__":
    main()
