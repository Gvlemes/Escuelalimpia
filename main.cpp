#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

void inicializarBaseDatosMovil() {
    // Busca la ruta segura de almacenamiento de la app en Android
    QString rutaAlmacenamiento = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(rutaAlmacenamiento);
    if (!dir.exists()) {
        dir.mkpath(rutaAlmacenamiento);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dir.filePath("escuela_limpia_movil.db"));

    if (db.open()) {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS reportes ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "fecha TEXT, "
                   "descripcion TEXT, "
                   "ruta_foto TEXT)");
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Inicializamos la BD en C++ de forma segura antes de cargar la UI
    inicializarBaseDatosMovil();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/appecologia/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        
    engine.load(url);

    return app.exec();
}
