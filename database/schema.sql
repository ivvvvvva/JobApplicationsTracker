PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS applications (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    company TEXT NOT NULL,
    position TEXT NOT NULL,
    job_url TEXT,
    date_applied TEXT NOT NULL,
    status TEXT NOT NULL,
    notes TEXT
);

CREATE TABLE IF NOT EXISTS questions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    application_id INTEGER NOT NULL,
    question TEXT NOT NULL,
    category TEXT NOT NULL,
    difficulty TEXT NOT NULL,
    my_answer TEXT,
    notes TEXT,
    practiced INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (application_id)
        REFERENCES applications(id)
        ON DELETE CASCADE
);
