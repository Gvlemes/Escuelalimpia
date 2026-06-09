import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtLocalStorage

ApplicationWindow {
    visible: true
    width: 360
    height: 640
    title: "EcoEscuela Mecatronica"

    property string rutaFotoSeleccionada: ""
    property var db: null
    property bool esAdminAutenticado: false

    Component.onCompleted: {
        db = LocalStorage.openDatabaseSync("EscuelaLimpiaDB", "1.0", "BD Escolar", 100000);
        db.transaction(function(tx) {
            tx.executeSql('CREATE TABLE IF NOT EXISTS reportes(id INTEGER PRIMARY KEY AUTOINCREMENT, fecha TEXT, descripcion TEXT, foto TEXT)');
        });
    }

    function guardarReporte(descripcion, fotoPath) {
        if (descripcion === "" || fotoPath === "") return false;
        db.transaction(function(tx) {
            var fechaActual = new Date().toISOString().replace('T', ' ').substr(0, 19);
            tx.executeSql('INSERT INTO reportes (fecha, descripcion, foto) VALUES (?, ?, ?)', [fechaActual, descripcion, fotoPath]);
        });
        actualizarListaAdmin();
        return true;
    }

    function actualizarListaAdmin() {
        if (!esAdminAutenticado) return;
        modeloReportes.clear();
        db.transaction(function(tx) {
            var rs = tx.executeSql('SELECT id, fecha, descripcion, foto FROM reportes ORDER BY id DESC');
            for (var i = 0; i < rs.rows.length; i++) {
                modeloReportes.append({
                    "idReporte": rs.rows.item(i).id.toString(),
                    "fechaReporte": rs.rows.item(i).fecha,
                    "descReporte": rs.rows.item(i).descripcion,
                    "fotoReporte": rs.rows.item(i).foto
                });
            }
        });
    }

    header: TabBar {
        id: bar
        width: parent.width
        TabButton { text: "⏱️ Degradación" }
        TabButton { text: "🚨 Reportar" }
        TabButton { text: "🧮 Calculadora" }
        TabButton { text: "👑 Admin" }
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: bar.currentIndex
        // --- PESTAÑA 1: DEGRADACIÓN ---
        Item {
            Column {
                anchors.fill: parent; anchors.margins: 20; spacing: 15
                Text { text: "⏱️ ¿Cuánto tardan en degradarse?"; font.pixelSize: 18; font.bold: true; anchors.horizontalCenter: parent.horizontalCenter }
                ComboBox {
                    id: comboMateriales
                    width: parent.width
                    model: [
                        "Selecciona un material...", "Chicle", "Botella de PET", "Lata de Aluminio", "Bolsa de Plastico",
                        "Carton y Papel", "Envase Tetra Brik (Jugos)", "Unicel / Telgopor", "Manzana / Organicos",
                        "Cables Jumpers (Protoboard)", "Pilas y Baterias Lipo", "Placas Fenolicas (PCBs)",
                        "Motores DC / Servomotores rotos", "Resistencias y Capacitores"
                    ]
                }
                Rectangle {
                    width: parent.width; height: 160; color: "#E8F5E9"; radius: 8; border.color: "#A5D6A7"
                    Text {
                        anchors.fill: parent; anchors.margins: 12; wrapMode: Text.Wrap; font.pixelSize: 14; color: "#1B5E20"
                        text: {
                            if (comboMateriales.currentText === "Chicle") return "Tarda 5 años en deshacerse debido a que el oxígeno lo endurece y reseca.";
                            if (comboMateriales.currentText === "Botella de PET") return "Tarda de 450 a 500 años en descomponerse. ¡Evita los refrescos plásticos!";
                            if (comboMateriales.currentText === "Lata de Aluminio") return "Tarda de 10 a 100 años en desaparecer por de oxidación química natural.";
                            if (comboMateriales.currentText === "Bolsa de Plastico") return "Tarda de 150 a 400 años. Solo se rompe en microplásticos nocivos.";
                            if (comboMateriales.currentText === "Carton y Papel") return "Tarda de 2 a 5 meses. Si se moja se degrada rápido, pero debe depositarse limpio para reciclar.";
                            if (comboMateriales.currentText === "Envase Tetra Brik (Jugos)") return "Tarda unos 30 años en degradarse por su compleja estructura de cartón, plástico y aluminio.";
                            if (comboMateriales.currentText === "Unicel / Telgopor") return "¡No se degrada nunca! Es 100% permanente y fragmentable, contaminando los patios escolares.";
                            if (comboMateriales.currentText === "Manzana / Organicos") return "Tarda de 2 a 4 semanas. Se degrada rápido y sirve de composta, pero genera mal olor en el salón.";
                            if (comboMateriales.currentText === "Cables Jumpers (Protoboard)") return "Tardan de 200 a 300 años. El cobre interno se oxida, pero el recubrimiento plástico de PVC dura siglos tirado.";
                            if (comboMateriales.currentText === "Pilas y Baterias Lipo") return "Tardan entre 500 y 1,000 años. Son extremadamente peligrosas; derraman mercurio, plomo y cadmio tóxico.";
                            if (comboMateriales.currentText === "Placas Fenolicas (PCBs)") return "Tardan más de 500 años. La baquelita y la fibra de vidrio con resina epóxica no son biodegradables.";
                            if (comboMateriales.currentText === "Motores DC / Servomotores rotos") return "Tardan de 100 a 500 años. Sus carcasas plásticas y engranajes metálicos resisten décadas a la intemperie.";
                            if (comboMateriales.currentText === "Resistencias y Capacitores") return "Tardan unos 200 a 400 años. Sus terminales metálicas se oxidan rápido, pero los cuerpos cerámicos y plásticos no.";
                            return "Por favor, selecciona una opción del menú para auditar su impacto ambiental.";
                        }
                    }
                }
            }
        }
        // --- PESTAÑA 2: REPORTAR ---
        Item {
            Column {
                anchors.fill: parent; anchors.margins: 20; spacing: 12
                Text { text: "🚨 Reportar Punto Sucio"; font.pixelSize: 18; font.bold: true; anchors.horizontalCenter: parent.horizontalCenter }
                TextField { id: txtDesc; width: parent.width; placeholderText: "Ej: Cables tirados en el lab de mecatronica" }
                Button { text: "📸 Seleccionar Foto de Evidencia"; width: parent.width; onClicked: fileDialog.open() }
                Rectangle {
                    width: parent.width; height: 160; color: "#F5F5F5"; border.color: "#DDD"; radius: 6; clip: true
                    Image { anchors.fill: parent; fillMode: Image.PreserveAspectFit; source: rutaFotoSeleccionada }
                    Text { text: "No se ha seleccionado foto"; visible: rutaFotoSeleccionada === ""; anchors.centerIn: parent; color: "#999" }
                }
                Button {
                    text: "📤 Enviar Reporte"; width: parent.width; highlighted: true
                    onClicked: {
                        if (guardarReporte(txtDesc.text, rutaFotoSeleccionada)) {
                            txtDesc.text = ""; rutaFotoSeleccionada = "";
                            popupMsg.mostrar("¡Reporte enviado exitosamente!");
                        } else { popupMsg.mostrar("Escribe una descripción y toma una foto."); }
                    }
                }
            }
            FileDialog { id: fileDialog; title: "Selecciona evidencia"; onAccepted: rutaFotoSeleccionada = fileDialog.selectedFile }
        }

        // --- PESTAÑA 3: CALCULADORA ---
        Item {
            Column {
                anchors.fill: parent; anchors.margins: 20; spacing: 15
                Text { text: "🧮 Huella de Plástico Anual"; font.pixelSize: 18; font.bold: true; anchors.horizontalCenter: parent.horizontalCenter }
                Text { text: "¿Cuántas botellas de plástico consumes por SEMANA?"; font.pixelSize: 13 }
                TextField { id: txtPet; width: parent.width; placeholderText: "Ej. 4"; inputMethodHints: Qt.ImhDigitsOnly }
                Button {
                    text: "🍃 Calcular Impacto"; width: parent.width
                    onClicked: {
                        var botellas = parseFloat(txtPet.text);
                        if (isNaN(botellas) || botellas < 0) { panelCalc.visible = false; popupMsg.mostrar("Ingresa un número válido."); return; }
                        var anual = botellas * 52; var kg = anual * 0.025;
                        lblCalcText.text = "📊 **Impacto estimado en 1 año:**\n\n• Consumirás unas **" + Math.floor(anual) + " botellas**.\n• Generas **" + kg.toFixed(2) + " kg** de basura plástica.\n\n💡 *Consejo:* Reemplázalas usando un termo de la escuela.";
                        panelCalc.visible = true;
                    }
                }
                Rectangle {
                    id: panelCalc; width: parent.width; height: 140; color: "#FFF3E0"; radius: 8; visible: false
                    Text { id: lblCalcText; anchors.fill: parent; anchors.margins: 12; wrapMode: Text.Wrap; font.pixelSize: 14; color: "#E65100" }
                }
            }
        }
        // --- PESTAÑA 4: ADMIN PROTEGIDO ---
        Item {
            Column {
                anchors.fill: parent; anchors.margins: 20; spacing: 15
                Column {
                    width: parent.width; spacing: 10; visible: !esAdminAutenticado
                    Text { text: "🔐 Acceso de Administrador"; font.pixelSize: 18; font.bold: true; anchors.horizontalCenter: parent.horizontalCenter }
                    TextField { id: txtPass; width: parent.width; echoMode: TextInput.Password; placeholderText: "Introduce la clave mecatronica" }
                    Button {
                        text: "Ingresar"; width: parent.width
                        onClicked: {
                            if (txtPass.text === "mecatronica") { esAdminAutenticado = true; txtPass.text = ""; actualizarListaAdmin(); }
                            else { popupMsg.mostrar("Clave incorrecta."); txtPass.text = ""; }
                        }
                    }
                }
                Column {
                    width: parent.width; height: parent.height; spacing: 10; visible: esAdminAutenticado
                    Text { text: "📋 Historial General de Reportes"; font.pixelSize: 16; font.bold: true }
                    ListView {
                        width: parent.width; height: 380; clip: true
                        model: ListModel { id: modeloReportes }
                        delegate: Rectangle {
                            width: parent.width; height: 260; color: "#FFFFFF"; border.color: "#EEE"; radius: 5
                            Column {
                                anchors.fill: parent; anchors.margins: 10; spacing: 5
                                Text { text: "📋 REPORTE NUMERO: " + idReporte; font.bold: true; color: "#333" }
                                Text { text: "📅 Fecha: " + fechaReporte; color: "#666" }
                                Text { text: "📝 Descripcion: " + descReporte; color: "#000"; wrapMode: Text.Wrap; width: parent.width }
                                Image { width: 140; height: 100; fillMode: Image.PreserveAspectFit; source: fotoReporte }
                                Text { text: "----------------------------------------------------"; color: "#CCC" }
                            }
                        }
                    }
                    Button { text: "🔄 Actualizar Tabla"; width: parent.width; onClicked: actualizarListaAdmin() }
                }
            }
        }
    }

    Dialog {
        id: popupMsg; anchors.centerIn: parent; title: "Aviso"; standardButtons: Dialog.Ok
        property alias text: lblMsg.text
        Text { id: lblMsg; padding: 10 }
        function mostrar(txt) { text = txt; open(); }
    }
}
