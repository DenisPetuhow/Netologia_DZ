#include <QCoreApplication>

#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Application started. Checking modules...";

    // --- 1. Проверяем модуль SQL ---
    // QSqlDatabase::addDatabase требует строку с типом драйвера (например, "QSQLITE").
    // Создание объекта подтверждает, что библиотека QtSql успешно слинкована.
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QSqlDatabase aa;
    qDebug() << "SQL Module: Object created.";
    // Проверяем, удалось ли загрузить драйвер и создать подключение
    if (aa.isValid()) {
        qDebug() << "SQL Module: Success! Object created and driver loaded.";
    } else {
        // Если ошибка — выводим предупреждение и текст ошибки
        qWarning() << "SQL Module: FAILED to create object.";
        // lastError().text() расскажет, что именно пошло не так
        qWarning() << aa.lastError().text();
    }


    // --- 2. Проверяем модуль Network ---
    // QNetworkAccessManager управляет отправкой запросов и получением ответов.
    // Создание экземпляра подтверждает, что библиотека QtNetwork успешно слинкована.
    QNetworkAccessManager manager;
    qDebug() << "Network Module: Object created.";


    return a.exec();
}
