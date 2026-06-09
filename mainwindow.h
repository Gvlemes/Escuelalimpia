#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QSqlDatabase>
#include <QTabWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboMateriales_currentTextChanged(const QString &material);
    void on_btnSeleccionarFoto_clicked();
    void on_btnEnviarReporte_clicked();
    void on_btnCalcular_clicked();
    void comprobarAccesoAdmin();
    void actualizarListaReportes();

private:
    QMap<QString, QString> datosMateriales;
    QString rutaFotoSeleccionada;
    QSqlDatabase db;
    bool esAdminAutenticado;

    // Componentes visuales
    QTabWidget *tabWidget;
    QComboBox *comboMateriales;
    QLabel *lblResultadoDegradacion;
    QLineEdit *txtDescripcion;
    QLabel *lblVistaPrevia;
    QLineEdit *txtCantidadPet;
    QLabel *lblResultadoCalculadora;

    // Componentes de la pestaña Admin
    QWidget *contenedorLoginAdmin;
    QWidget *contenedorListaAdmin;
    QLineEdit *txtPasswordAdmin;
    QListWidget *listaReportes;

    void inicializarBaseDatos();
    void configurarMateriales();
    void crearInterfazGrafica();
};
#endif // MAINWINDOW_H
