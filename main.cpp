#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>

void inicializarBaseDatosMovil() {
    // Usamos la ruta de caché temporal que Android 14 nunca bloquea
    QString rutaAlmacenamiento = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
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

    inicializarBaseDatosMovil();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/escuelamovil/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        
    engine.load(url);

    return app.exec();
}
