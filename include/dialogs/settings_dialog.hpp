#ifndef SETTINGS_DIALOG_HPP
#define SETTINGS_DIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <canvas/settings.hpp>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    virtual ~SettingsDialog();

private slots:
    void saveSettings();
private:
    Ui::SettingsDialog *ui;
    QSettings mSettings;

    void changeApiHost();
    void changeApiPort();
};

#endif // SETTINGS_DIALOG_HPP
