#include <QtSql>

const auto CLASSES_SQL = QString(R"(
    CREATE TABLE classes(
        id INTEGER PRIMARY KEY,
        title VARCHAR UNIQUE NOT NULL,
        canvas_id INTEGER
    )
    )");

const auto STUDENTS_SQL = QString(R"(
    create table students(
        id INTEGER PRIMARY KEY,
        name VARCHAR NOT NULL,
        class_id INTEGER REFERENCES classes(id) NOT NULL,
        canvas_id INTEGER,
        sis_id INTEGER,
        sis_username VARCHAR,
        section VARCHAR
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

        q.prepare("INSERT INTO students (name, class_id) VALUES (?, ?)");

        for (auto classId : {class1Id, class2Id}) {
            for (int i = 0; i < 4; i++) {
                q.addBindValue(QString("Student ") + static_cast<char>('1' + i));
                q.addBindValue(classId);
                if (!q.exec())
                    return q.lastError();
            }
        }
    }

    return QSqlError();
}