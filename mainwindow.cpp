#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSettings>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <WinUser.h>
#include <dwmapi.h>
#endif

const int DWMWA_MICA_EFFECT = 1029;

// Valores para el backdrop
const int DWMSBT_MICA = 3; // 2 Mica, 3 Acrylic

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    #ifdef Q_OS_WIN // Only for Windows
    this->setAttribute(Qt::WA_TranslucentBackground);
    HWND hwnd = (HWND)this->winId();

    QOperatingSystemVersion version = QOperatingSystemVersion::current();
    int build = version.microVersion(); // microVersion() es el número de build

    if (build < 22000) {
        return;
    }

    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
    int appsUseLightTheme = settings.value("AppsUseLightTheme", 1).toInt();
    int isDark = (appsUseLightTheme == 0) ? 1 : 0;

    DwmSetWindowAttribute(
        hwnd,
        DWMWA_USE_IMMERSIVE_DARK_MODE,
        &isDark, // 1 para oscuro, 0 para claro
        sizeof(isDark)
        );

    MARGINS margins = {-1}; // -1 = toda la ventana
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // Aplicar el efecto MICA (lógica de build) ---
    int backdropValue = DWMSBT_MICA; // 2

    if (build >= 22621) { // Si es 22H2 o más nuevo
        // Usar la API nueva (DWMWA_SYSTEMBACKDROP_TYPE)
        DwmSetWindowAttribute(
            hwnd,
            DWMWA_SYSTEMBACKDROP_TYPE, // Atributo 38
            &backdropValue,
            sizeof(backdropValue)
            );
    } else { // Si es 21H2 (el primer Win11)
        // Usar la API antigua (DWMWA_MICA_EFFECT)
        int micaValue = 1; // 1 = Habilitar
        DwmSetWindowAttribute(
            hwnd,
            DWMWA_MICA_EFFECT, // Atributo 1029
            &micaValue,
            sizeof(micaValue)
            );
    }
    #endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this, "Prueba", "esto es un mensaje");
}

