#include <QtSql>

// TODO: Extend table schema
const auto CLASSES_SQL = QString(R"(
    CREATE TABLE classes(
        id INTEGER PRIMARY KEY,
        title VARCHAR NOT NULL,
        canvas_id INTEGER,
        sis_id INTEGER
    )
    )");

const auto STUDENTS_SQL = QString(R"(
    create table students(
        id integer primary key,
        name varchar,
        class integer,
        canvas_id integer,
        sis_id integer
    )
)");

const auto GROUPS_SQL = QString(R"(
    create table groups(
        class_id integer,
        student_id integer,
        group_name varchar
    )
)");

QSqlError initDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    QSqlQuery q;

    if (!tables.contains("classes", Qt::CaseInsensitive)) {
        if (!q.exec(CLASSES_SQL))
            return q.lastError();
        q.prepare("INSERT INTO classes (title) VALUES (?)");
        q.addBindValue("Test");
        if (!q.exec())
            return q.lastError();
    }

    if (!tables.contains("students", Qt::CaseInsensitive))
        if (!q.exec(STUDENTS_SQL))
            return q.lastError();

    if (!tables.contains("groups", Qt::CaseInsensitive))
        if (!q.exec(GROUPS_SQL))
            return q.lastError();

    // TODO: Add Test Data

    return QSqlError();
}