#include <QtSql>

// TODO: Extend table schema
const auto CLASSES_SQL = QString(R"(
    create table classes(
        id integer primary key,
        title varchar not null,
        canvas_id integer,
        sis_id integer
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

    if (tables.contains("classes", Qt::CaseInsensitive))
        if (!q.exec(CLASSES_SQL))
            return q.lastError();

    if (tables.contains("students", Qt::CaseInsensitive))
        if (!q.exec(STUDENTS_SQL))
            return q.lastError();

    if (tables.contains("groups", Qt::CaseInsensitive))
        if (!q.exec(GROUPS_SQL))
            return q.lastError();

    // TODO: Add Test Data

    return QSqlError();
}