#ifndef PDSETTINGSDIALOG_H
#define PDSETTINGSDIALOG_H

#include <QString>
#include <QVariant>
#include <QMap>

#include <QWidget>
#include <QDialog>

namespace Ui {
    class pdSettingsDialog;
}

class pdSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit pdSettingsDialog(const QMap<QString, QVariant> &preferences,
                              QWidget *parent = 0);
    ~pdSettingsDialog();

    // Getters:
    QMap<QString, QVariant> preferences();
    QStringList downloadFormatsList();
    QStringList redistributeFormatsList();

    // Setters:
    void setPreferences(const QMap<QString, QVariant> &prefs);

private:
    Ui::pdSettingsDialog *ui;
    QMap<QString, QVariant> prefs;

private slots:
    void on_checkBoxRALL_toggled(bool checked);
    void on_checkBoxDALL_toggled(bool checked);
//    void on_accepted();
};

#endif // PDSETTINGSDIALOG_H
