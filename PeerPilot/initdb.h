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
        id INTEGER PRIMARY KEY,
        name VARCHAR,
        class_id INTEGER REFERENCES classes(id),
        canvas_id INTEGER,
        sis_id INTEGER
    )
)");

const auto GROUPS_SQL = QString(R"(
    create table groups(
        class_id INTEGER REFERENCES classes(id),
        student_id INTEGER REFERENCES students(id),
        name VARCHAR
    )
)");

const auto TEST_DATA = true;

QSqlError initDb() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    QSqlQuery q;

    if (!tables.contains("classes", Qt::CaseInsensitive))
        if (!q.exec(CLASSES_SQL))
            return q.lastError();

    if (!tables.contains("students", Qt::CaseInsensitive))
        if (!q.exec(STUDENTS_SQL))
            return q.lastError();

    if (!tables.contains("groups", Qt::CaseInsensitive))
        if (!q.exec(GROUPS_SQL))
            return q.lastError();

    if (TEST_DATA) {
        q.prepare("INSERT INTO classes (title) VALUES (?)");

        q.addBindValue("Class 1");
        if (!q.exec())
            return q.lastError();
        QString class1Id = q.lastInsertId().toString();

        q.addBindValue("Class 2");
        if (!q.exec())
            return q.lastError();
        QString class2Id = q.lastInsertId().toString();


        for (auto classId : {class1Id, class2Id}) {
            for (int i = 0; i < 4; i++) {
                q.prepare("INSERT INTO students (name, class_id) VALUES (?, ?)");
                q.addBindValue(QString("Student ") + static_cast<char>('1' + i));
                q.addBindValue(classId);
                if (!q.exec())
                    return q.lastError();

                q.prepare("INSERT INTO groups (class_id, student_id, name) VALUES (?, ?, ?)");
                q.addBindValue(classId);
                q.addBindValue(1 + i + (classId == class2Id ? 4 : 0));
                q.addBindValue(QString("Group ") + static_cast<char>('1' + i/2));
                if (!q.exec())
                    return q.lastError();
            }
        }
    }

    return QSqlError();
}