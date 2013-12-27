#include "include/dialogs/settings_dialog.hpp"
#include "ui_settings_dialog.h"
#include "viewport.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    QSettings settings;

    QString const defaultHost =
            QString::fromStdString(Canvas::Settings::get("canvas_host"));
    QString const defaultPort =
            QString::fromStdString(Canvas::Settings::get("canvas_port"));

    ui->canvasApiHostText->setText(settings.value("canvas/host", defaultHost).toString());
    ui->canvasApiPortText->setText(settings.value("canvas/port", defaultPort).toString());

    connect(ui->canvasApiHostText, SIGNAL(editingFinished()),
            this, SLOT(saveSettings()));

    connect(ui->canvasApiPortText, SIGNAL(editingFinished()),
            this, SLOT(saveSettings()));
}

SettingsDialog::~SettingsDialog()
{
    saveSettings();
    delete ui;
}

void SettingsDialog::saveSettings()
{
    mSettings.setValue("canvas/host", ui->canvasApiHostText->text());
    mSettings.setValue("canvas/port", ui->canvasApiPortText->text());

    changeApiHost();
    changeApiPort();

    Viewport::singleton().setStatus("Settings have been saved.");
}

void SettingsDialog::changeApiHost()
{
    Canvas::Settings::set("canvas_host",
                          mSettings.value("canvas/host").toString().toStdString());
}

void SettingsDialog::changeApiPort()
{
    Canvas::Settings::set("canvas_port",
                          mSettings.value("canvas/port").toString().toStdString());
}
