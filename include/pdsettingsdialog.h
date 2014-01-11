#ifndef PDSETTINGSDIALOG_H
#define PDSETTINGSDIALOG_H

#include <QtGui>
#include <QDialog>

namespace Ui {
    class pdSettingsDialog;
}

class pdSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit pdSettingsDialog(QMap<QString, QVariant> preferences,
                              QWidget *parent = 0);
    ~pdSettingsDialog();

    // Getters:
    QMap<QString, QVariant> preferences();
    QStringList downloadFormatsList();
    QStringList redistributeFormatsList();

    // Setters:
    void setPreferences(QMap<QString, QVariant> prefs);

private:
    Ui::pdSettingsDialog *ui;
    QMap<QString, QVariant> prefs;

private slots:
    void on_checkBoxRALL_toggled(bool checked);
    void on_checkBoxDALL_toggled(bool checked);
//    void on_accepted();
};

#endif // PDSETTINGSDIALOG_H
