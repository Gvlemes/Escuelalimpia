#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , esAdminAutenticado(false)
{
    this->setWindowTitle("EcoEscuela - Concientizacion");
    this->resize(450, 600);

    crearInterfazGrafica();
    inicializarBaseDatos();
    configurarMateriales();
}

MainWindow::~MainWindow()
{
}
void MainWindow::crearInterfazGrafica()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    tabWidget = new QTabWidget(this);

    QFont fontTitulo;
    fontTitulo.setPointSize(14);
    fontTitulo.setBold(true);

    // --- PESTAÑA 1: DEGRADACIÓN ---
    QWidget *tabDegradacion = new QWidget();
    QVBoxLayout *layoutTab1 = new QVBoxLayout(tabDegradacion);
    layoutTab1->setSpacing(15);

    QLabel *lblTitulo1 = new QLabel("⏱️ ¿Cuánto tardan en degradarse?", tabDegradacion);
    lblTitulo1->setFont(fontTitulo);
    lblTitulo1->setAlignment(Qt::AlignCenter);

    QLabel *lblInstruccion1 = new QLabel("Selecciona un material:", tabDegradacion);
    comboMateriales = new QComboBox(tabDegradacion);

    lblResultadoDegradacion = new QLabel(tabDegradacion);
    lblResultadoDegradacion->setStyleSheet("background-color: #E8F5E9; padding: 15px; border-radius: 8px; color: #1B5E20;");
    lblResultadoDegradacion->setWordWrap(true);

    layoutTab1->addWidget(lblTitulo1);
    layoutTab1->addWidget(lblInstruccion1);
    layoutTab1->addWidget(comboMateriales);
    layoutTab1->addWidget(lblResultadoDegradacion);
    layoutTab1->addStretch();
    tabWidget->addTab(tabDegradacion, "⏱️ Degradacion");

    // --- PESTAÑA 2: REPORTAR ---
    QWidget *tabReportar = new QWidget();
    QVBoxLayout *layoutTab2 = new QVBoxLayout(tabReportar);

    QLabel *lblTitulo2 = new QLabel("🚨 Reportar Punto Sucio", tabReportar);
    lblTitulo2->setFont(fontTitulo);
    lblTitulo2->setAlignment(Qt::AlignCenter);

    QLabel *lblInstruccion2 = new QLabel("Descripción de la zona:", tabReportar);
    txtDescripcion = new QLineEdit(tabReportar);
    txtDescripcion->setPlaceholderText("Ej: Basura acumulada en las canchas");

    QPushButton *btnSeleccionarFoto = new QPushButton("📸 Seleccionar Foto", tabReportar);

    lblVistaPrevia = new QLabel("No se ha seleccionado ninguna foto", tabReportar);
    lblVistaPrevia->setStyleSheet("border: 1px dashed #aaa; min-height: 150px; background: #f9f9f9;");
    lblVistaPrevia->setAlignment(Qt::AlignCenter);

    QPushButton *btnEnviarReporte = new QPushButton("📤 Enviar Reporte a Base de Datos", tabReportar);
    btnEnviarReporte->setStyleSheet("background-color: #D32F2F; color: white; font-weight: bold; padding: 8px;");

    layoutTab2->addWidget(lblTitulo2);
    layoutTab2->addWidget(lblInstruccion2);
    layoutTab2->addWidget(txtDescripcion);
    layoutTab2->addWidget(btnSeleccionarFoto);
    layoutTab2->addWidget(lblVistaPrevia);
    layoutTab2->addWidget(btnEnviarReporte);
    tabWidget->addTab(tabReportar, "🚨 Reportar");

    // --- PESTAÑA 3: CALCULADORA ---
    QWidget *tabCalculadora = new QWidget();
    QVBoxLayout *layoutTab3 = new QVBoxLayout(tabCalculadora);
    layoutTab3->setSpacing(15);

    QLabel *lblTitulo3 = new QLabel("🧮 Huella Ecológica de Plástico", tabCalculadora);
    lblTitulo3->setFont(fontTitulo);
    lblTitulo3->setAlignment(Qt::AlignCenter);

    QLabel *lblInstruccion3 = new QLabel("¿Cuántas botellas de PET consumes a la SEMANA?", tabCalculadora);
    txtCantidadPet = new QLineEdit(tabCalculadora);
    txtCantidadPet->setPlaceholderText("Ingresa un número (Ej. 4)");

    QPushButton *btnCalcular = new QPushButton("🍃 Calcular Huella Anual", tabCalculadora);
    btnCalcular->setStyleSheet("background-color: #2E7D32; color: white; font-weight: bold; padding: 8px;");

    lblResultadoCalculadora = new QLabel(tabCalculadora);
    lblResultadoCalculadora->setStyleSheet("background-color: #FFF3E0; padding: 15px; border-radius: 8px; color: #E65100;");
    lblResultadoCalculadora->setWordWrap(true);

    layoutTab3->addWidget(lblTitulo3);
    layoutTab3->addWidget(lblInstruccion3);
    layoutTab3->addWidget(txtCantidadPet);
    layoutTab3->addWidget(btnCalcular);
    layoutTab3->addWidget(lblResultadoCalculadora);
    layoutTab3->addStretch();
    tabWidget->addTab(tabCalculadora, "🧮 Calculadora");

    // --- PESTAÑA 4: PANEL ADMINISTRADOR ---
    QWidget *tabAdmin = new QWidget();
    QVBoxLayout *layoutTab4 = new QVBoxLayout(tabAdmin);

    contenedorLoginAdmin = new QWidget(tabAdmin);
    QVBoxLayout *layoutLogin = new QVBoxLayout(contenedorLoginAdmin);
    QLabel *lblLoginPrompt = new QLabel("🔐 Acceso restringido para Administradores.\nIntroduce la clave de acceso:", contenedorLoginAdmin);
    lblLoginPrompt->setAlignment(Qt::AlignCenter);
    txtPasswordAdmin = new QLineEdit(contenedorLoginAdmin);
    txtPasswordAdmin->setEchoMode(QLineEdit::Password);
    txtPasswordAdmin->setPlaceholderText("Clave de seguridad");
    QPushButton *btnLoginAdmin = new QPushButton("Ingresar", contenedorLoginAdmin);
    btnLoginAdmin->setStyleSheet("background-color: #1976D2; color: white; font-weight: bold; padding: 6px;");

    layoutLogin->addWidget(lblLoginPrompt);
    layoutLogin->addWidget(txtPasswordAdmin);
    layoutLogin->addWidget(btnLoginAdmin);
    layoutLogin->addStretch();

    contenedorListaAdmin = new QWidget(tabAdmin);
    QVBoxLayout *layoutLista = new QVBoxLayout(contenedorListaAdmin);
    QLabel *lblListaTitulo = new QLabel("📋 Registro General de Puntos Sucios", contenedorListaAdmin);
    lblListaTitulo->setFont(fontTitulo);
    listaReportes = new QListWidget(contenedorListaAdmin);
    listaReportes->setStyleSheet("background-color: #FFFFFF; color: #000000; font-size: 12px; border: 1px solid #CCC;");
    QPushButton *btnActualizarListado = new QPushButton("🔄 Actualizar Tabla", contenedorListaAdmin);

    layoutLista->addWidget(lblListaTitulo);
    layoutLista->addWidget(listaReportes);
    layoutLista->addWidget(btnActualizarListado);

    contenedorListaAdmin->setVisible(false);

    layoutTab4->addWidget(contenedorLoginAdmin);
    layoutTab4->addWidget(contenedorListaAdmin);
    tabWidget->addTab(tabAdmin, "👑 Admin");

    mainLayout->addWidget(tabWidget);
    setCentralWidget(centralWidget);

    connect(comboMateriales, &QComboBox::currentTextChanged, this, &MainWindow::on_comboMateriales_currentTextChanged);
    connect(btnSeleccionarFoto, &QPushButton::clicked, this, &MainWindow::on_btnSeleccionarFoto_clicked);
    connect(btnEnviarReporte, &QPushButton::clicked, this, &MainWindow::on_btnEnviarReporte_clicked);
    connect(btnCalcular, &QPushButton::clicked, this, &MainWindow::on_btnCalcular_clicked);
    connect(btnLoginAdmin, &QPushButton::clicked, this, &MainWindow::comprobarAccesoAdmin);
    connect(btnActualizarListado, &QPushButton::clicked, this, &MainWindow::actualizarListaReportes);
}
void MainWindow::comprobarAccesoAdmin()
{
    if (txtPasswordAdmin->text() == "mecatronica") {
        esAdminAutenticado = true;
        contenedorLoginAdmin->setVisible(false);
        contenedorListaAdmin->setVisible(true);
        actualizarListaReportes();
    } else {
        QMessageBox::critical(this, "Acceso Denegado", "La clave introducida es incorrecta.");
        txtPasswordAdmin->clear();
    }
}

void MainWindow::actualizarListaReportes()
{
    if (!esAdminAutenticado) return;

    listaReportes->clear();
    QSqlQuery query("SELECT id, fecha, descripcion, ruta_foto FROM reportes ORDER BY id DESC");

    while (query.next()) {
        QString id = query.value(0).toString();
        QString fecha = query.value(1).toString();
        QString desc = query.value(2).toString();
        QString foto = query.value(3).toString();

        QWidget *contenedorFila = new QWidget(listaReportes);
        QVBoxLayout *layoutFila = new QVBoxLayout(contenedorFila);
        layoutFila->setContentsMargins(10, 10, 10, 10);
        layoutFila->setSpacing(5);

        QLabel *lblTextoData = new QLabel(contenedorFila);
        lblTextoData->setStyleSheet("color: #000000; font-weight: normal;");
        QString formatoTexto = QString("📋 REPORTE NUMERO: %1\n"
                                       "📅 Fecha: %2\n"
                                       "📝 Descripcion: %3")
                                   .arg(id).arg(fecha).arg(desc);
        lblTextoData->setText(formatoTexto);
        layoutFila->addWidget(lblTextoData);

        QLabel *lblFotoReporte = new QLabel(contenedorFila);
        QPixmap pixmap(foto);

        if (!pixmap.isNull()) {
            lblFotoReporte->setPixmap(pixmap.scaled(200, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            lblFotoReporte->setText("⚠️ [No se pudo cargar la imagen original o fue borrada]");
            lblFotoReporte->setStyleSheet("color: #D32F2F; font-style: italic;");
        }
        layoutFila->addWidget(lblFotoReporte);

        QLabel *lblDivisor = new QLabel("----------------------------------------------------------------------", contenedorFila);
        lblDivisor->setStyleSheet("color: #999;");
        layoutFila->addWidget(lblDivisor);

        QListWidgetItem *itemLista = new QListWidgetItem(listaReportes);
        itemLista->setSizeHint(contenedorFila->sizeHint());
        listaReportes->addItem(itemLista);
        listaReportes->setItemWidget(itemLista, contenedorFila);
    }

    if(listaReportes->count() == 0) {
        listaReportes->addItem("No se han registrado reportes en la base de datos aun.");
    }
}
void MainWindow::inicializarBaseDatos()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("escuela_limpia.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Error de BD", "No se pudo abrir la base de datos.");
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS reportes ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "fecha TEXT, "
               "descripcion TEXT, "
               "ruta_foto TEXT)");
}

void MainWindow::configurarMateriales()
{
    comboMateriales->clear();
    datosMateriales.clear();

    datosMateriales["Selecciona un material..."] = QString::fromUtf8("Por favor, elige una opción del menú.");

    // --- BÁSICOS DE LA ESCUELA ---
    datosMateriales["Chicle"] = QString::fromUtf8("Tarda 5 años en deshacerse debido a que el oxígeno lo endurece y reseca.");
    datosMateriales["Botella de PET"] = QString::fromUtf8("Tarda de 450 a 500 años en descomponerse. ¡Evita los refrescos plásticos!");
    datosMateriales["Lata de Aluminio"] = QString::fromUtf8("Tarda de 10 a 100 años en desaparecer por oxidación química natural.");
    datosMateriales["Bolsa de Plastico"] = QString::fromUtf8("Tarda de 150 a 400 años. Solo se rompe en microplásticos nocivos.");
    datosMateriales["Carton y Papel"] = QString::fromUtf8("Tarda de 2 a 5 meses. Si se moja se degrada rápido, pero debe depositarse limpio para reciclar.");
    datosMateriales["Jugos"] = QString::fromUtf8("Tarda unos 30 años en degradarse por su compleja estructura de cartón, plástico y aluminio.");
    datosMateriales["Unicel / Telgopor"] = QString::fromUtf8("¡No se degrada nunca! Es 100% permanente y fragmentable, contaminando los patios escolares.");
    datosMateriales["Manzana / Residuos Organicos"] = QString::fromUtf8("Tarda de 2 a 4 semanas. Se degrada rápido y sirve de composta, pero genera mal olor en el salón.");

    // --- COMPONENTES DE MECATRÓNICA ---
    datosMateriales["Cables Jumpers (Protoboard)"] = QString::fromUtf8("Tardan de 200 a 300 años. El cobre interno se oxida, pero el recubrimiento plástico de PVC dura siglos tirado.");
    datosMateriales["Pilas y Baterias Lipo"] = QString::fromUtf8("Tardan entre 500 y 1,000 años. Son extremadamente peligrosas; derraman mercurio, plomo y cadmio tóxico.");
    datosMateriales["Placas Fenolicas (PCBs)"] = QString::fromUtf8("Tardan más de 500 años. La baquelita y la fibra de vidrio con resina epóxica no son biodegradables.");
    datosMateriales["Motores DC / Servomotores rotos"] = QString::fromUtf8("Tardan de 100 a 500 años. Sus carcasas plásticas y engranajes metálicos resisten décadas a la intemperie.");
    datosMateriales["Resistencias y Capacitores"] = QString::fromUtf8("Tardan unos 200 a 400 años. Sus terminales metálicas se oxidan rápido, pero los cuerpos cerámicos y plásticos no.");

    comboMateriales->addItems(datosMateriales.keys());
    lblResultadoDegradacion->setText(datosMateriales["Selecciona un material..."]);
}

void MainWindow::on_comboMateriales_currentTextChanged(const QString &material)
{
    if(datosMateriales.contains(material)) {
        lblResultadoDegradacion->setText(datosMateriales[material]);
    }
}

void MainWindow::on_btnSeleccionarFoto_clicked()
{
    rutaFotoSeleccionada = QFileDialog::getOpenFileName(this, "Seleccionar Evidencia", "", "Imagenes (*.png *.jpg *.jpeg)");
    if (!rutaFotoSeleccionada.isEmpty()) {
        QPixmap pixmap(rutaFotoSeleccionada);
        lblVistaPrevia->setPixmap(pixmap.scaled(lblVistaPrevia->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::on_btnEnviarReporte_clicked()
{
    QString descripcion = txtDescripcion->text().trimmed();

    if (descripcion.isEmpty() || rutaFotoSeleccionada.isEmpty()) {
        QMessageBox::warning(this, "Aviso", QString::fromUtf8("Escribe una descripción y selecciona una fotografía."));
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO reportes (fecha, descripcion, ruta_foto) VALUES (:fecha, :desc, :foto)");
    query.bindValue(":fecha", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":desc", descripcion);
    query.bindValue(":foto", rutaFotoSeleccionada);

    if (query.exec()) {
        QMessageBox::information(this, "Guardado", QString::fromUtf8("El reporte se almacenó en la base de datos de manera correcta."));
        txtDescripcion->clear();
        lblVistaPrevia->setText("No se ha seleccionado ninguna foto");
        rutaFotoSeleccionada = "";
        actualizarListaReportes();
    } else {
        QMessageBox::critical(this, "Error", "Fallo al guardar: " + query.lastError().text());
    }
}

void MainWindow::on_btnCalcular_clicked()
{
    bool ok;
    double botellasSemana = txtCantidadPet->text().toDouble(&ok);

    if (!ok || botellasSemana < 0) {
        QMessageBox::warning(this, "Error", QString::fromUtf8("Ingresa un número válido de botellas."));
        return;
    }

    double botellasAno = botellasSemana * 52;
    double pesoTotalKg = botellasAno * 0.025;

    QString texto = QString::fromUtf8("📊 Impacto estimado en 1 año:\n\n"
                                      " Consumirás aproximadamente %1 botellas.\n"
                                      " Basura plástica acumulada: %2 kg.\n\n"
                                      " Reemplaza el plástico usando termos reutilizables.")
                        .arg(static_cast<int>(botellasAno))
                        .arg(pesoTotalKg, 0, 'f', 2);

    lblResultadoCalculadora->setText(texto);
}
