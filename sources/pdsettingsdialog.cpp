#include "../headers/pdsettingsdialog.h"
#include "ui_pdsettingsdialog.h"

/*!
    \class pdSettingsDialog
    \brief Application's settings dialog.

    Class responsible for displaying and handling the settings dialog.
    */

/*!
    \fn pdSettingsDialog::pdSettingsDialog(QMap<QString, QVariant> preferences, QWidget *parent)

    Constructs the settings dialog, using \a preferences and optional \a parent.
    */
pdSettingsDialog::pdSettingsDialog(QMap<QString, QVariant> preferences, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pdSettingsDialog)
{
    ui->setupUi(this);
    setLayout(ui->mainLayout);

    // Validation:
    QRegExp formatRegExp("([a-z0-9]{3,4}\\s)*", Qt::CaseInsensitive);
    ui->lineEditDCustomFormat->setValidator(new QRegExpValidator(formatRegExp, this));
    ui->lineEditRCustomFormat->setValidator(new QRegExpValidator(formatRegExp, this));

    setPreferences(preferences);
}

/*!
    \internal

    Destructor. Deletes the UI.
    */
pdSettingsDialog::~pdSettingsDialog()
{
    delete ui;
}

/*!
    \fn pdSettingsDialog::preferences()

    Returns all the preferences.
    */
QMap<QString, QVariant> pdSettingsDialog::preferences()
{
    QMap<QString, QVariant> result;

    // Get download preferences:
    result.insert("dFormats", ui->lineEditDCustomFormat->text().split(" ", QString::SkipEmptyParts));
    result.insert("dAll", ui->checkBoxDALL->isChecked());
    result.insert("dBmp", ui->checkBoxDBMP->isChecked());
    result.insert("dCanon", ui->checkBoxDCanon->isChecked());
    result.insert("dDng", ui->checkBoxDDNG->isChecked());
    result.insert("dJpg", ui->checkBoxDJPG->isChecked());
    result.insert("dNikon", ui->checkBoxDNikon->isChecked());
    result.insert("dOlympus",  ui->checkBoxDOlympus->isChecked());
    result.insert("dPentax", ui->checkBoxDPentax->isChecked());
    result.insert("dPng", ui->checkBoxDPNG->isChecked());
    result.insert("dSigma",  ui->checkBoxDSigma->isChecked());
    result.insert("dSony", ui->checkBoxDSony->isChecked());
    result.insert("dTiff",  ui->checkBoxDTIFF->isChecked());

    // Get redistribution preferences:
    result.insert("rFormats", ui->lineEditRCustomFormat->text().split(" ", QString::SkipEmptyParts));
    result.insert("rAll", ui->checkBoxRALL->isChecked());
    result.insert("rBmp", ui->checkBoxRBMP->isChecked());
    result.insert("rCanon", ui->checkBoxRCanon->isChecked());
    result.insert("rDng", ui->checkBoxRDNG->isChecked());
    result.insert("rJpg", ui->checkBoxRJPG->isChecked());
    result.insert("rNikon", ui->checkBoxRNikon->isChecked());
    result.insert("rOlympus",  ui->checkBoxROlympus->isChecked());
    result.insert("rPentax", ui->checkBoxRPentax->isChecked());
    result.insert("rPng", ui->checkBoxRPNG->isChecked());
    result.insert("rSigma",  ui->checkBoxRSigma->isChecked());
    result.insert("rSony", ui->checkBoxRSony->isChecked());
    result.insert("rTiff",  ui->checkBoxRTIFF->isChecked());

    return result;
}

/*!
    \fn pdSettingsDialog::setPreferences(QMap<QString, QVariant> prefs)

    Sets all the preferences (\a prefs).
    */
void pdSettingsDialog::setPreferences(QMap<QString, QVariant> prefs)
{
    // Set download preferences:
    ui->lineEditDCustomFormat->setText(prefs.value("dFormats", "").toStringList().join(" "));
    ui->checkBoxDALL->setChecked(prefs.value("dAll", FALSE).toBool());
    ui->checkBoxDBMP->setChecked(prefs.value("dBmp", FALSE).toBool());
    ui->checkBoxDCanon->setChecked(prefs.value("dCanon", FALSE).toBool());
    ui->checkBoxDDNG->setChecked(prefs.value("dDng", FALSE).toBool());
    ui->checkBoxDJPG->setChecked(prefs.value("dJpg", FALSE).toBool());
    ui->checkBoxDNikon->setChecked(prefs.value("dNikon", FALSE).toBool());
    ui->checkBoxDOlympus->setChecked(prefs.value("dOlympus", FALSE).toBool());
    ui->checkBoxDPentax->setChecked(prefs.value("dPentax", FALSE).toBool());
    ui->checkBoxDPNG->setChecked(prefs.value("dPng", FALSE).toBool());
    ui->checkBoxDSigma->setChecked(prefs.value("dSigma", FALSE).toBool());
    ui->checkBoxDSony->setChecked(prefs.value("dSony", FALSE).toBool());
    ui->checkBoxDTIFF->setChecked(prefs.value("dTiff", FALSE).toBool());

    // Set redistribution preferences:
    ui->lineEditRCustomFormat->setText(prefs.value("rFormats", "").toStringList().join(" "));
    ui->checkBoxRALL->setChecked(prefs.value("rAll", FALSE).toBool());
    ui->checkBoxRBMP->setChecked(prefs.value("rBmp", FALSE).toBool());
    ui->checkBoxRCanon->setChecked(prefs.value("rCanon", FALSE).toBool());
    ui->checkBoxRDNG->setChecked(prefs.value("rDng", FALSE).toBool());
    ui->checkBoxRJPG->setChecked(prefs.value("rJpg", FALSE).toBool());
    ui->checkBoxRNikon->setChecked(prefs.value("rNikon", FALSE).toBool());
    ui->checkBoxROlympus->setChecked(prefs.value("rOlympus", FALSE).toBool());
    ui->checkBoxRPentax->setChecked(prefs.value("rPentax", FALSE).toBool());
    ui->checkBoxRPNG->setChecked(prefs.value("rPng", FALSE).toBool());
    ui->checkBoxRSigma->setChecked(prefs.value("rSigma", FALSE).toBool());
    ui->checkBoxRSony->setChecked(prefs.value("rSony", FALSE).toBool());
    ui->checkBoxRTIFF->setChecked(prefs.value("rTiff", FALSE).toBool());
}

/*!
    \fn pdSettingsDialog::downloadFormatsList()

    Returns format list for downloader.
    */
QStringList pdSettingsDialog::downloadFormatsList()
{
    QStringList result = ui->lineEditDCustomFormat->text().split(" ", QString::SkipEmptyParts);

    foreach(QString x, result)
    {
        x.prepend("*.");
    }

//    "dAll", ui->checkBoxDALL->isChecked());
    if (ui->checkBoxDBMP->isChecked())
        result << "*.bmp";
    if (ui->checkBoxDCanon->isChecked())
        result << "*.cr2";
    if (ui->checkBoxDDNG->isChecked())
        result << "*.dng";
    if (ui->checkBoxDJPG->isChecked())
        result << "*.jpg" << "*.jpeg";
    if (ui->checkBoxDNikon->isChecked())
        result << "*.nef";
    if (ui->checkBoxDOlympus->isChecked())
        result << "*.orf";
    if (ui->checkBoxDPentax->isChecked())
        result << "*.pef";
    if (ui->checkBoxDPNG->isChecked())
        result << "*.png";
    if (ui->checkBoxDSigma->isChecked())
        result << "*.x3f";
    if (ui->checkBoxDSony->isChecked())
        result << "*.arw" << "*.sr2" << "*.srf";
    if (ui->checkBoxDTIFF->isChecked())
        result << "*.tif" << "*.tiff";

    return result;
}

/*!
    \fn pdSettingsDialog::redistributeFormatsList()

    Returns format list for redistributor.
    */
QStringList pdSettingsDialog::redistributeFormatsList()
{
    QStringList result = ui->lineEditRCustomFormat->text().split(" ", QString::SkipEmptyParts);

    foreach(QString x, result)
    {
        x.prepend("*.");
    }

    if (ui->checkBoxDBMP->isChecked())
        result << "*.bmp";
    if (ui->checkBoxDDNG->isChecked())
        result << "*.dng";
    if (ui->checkBoxDJPG->isChecked())
        result << "*.jpg" << "*.jpeg";
    if (ui->checkBoxDPNG->isChecked())
        result << "*.png";
    if (ui->checkBoxDTIFF->isChecked())
        result << "*.tif" << "*.tiff";

    return result;
}

/*!
    \fn pdSettingsDialog::on_checkBoxDALL_toggled(bool checked)

    Handles the "check all" checkbox (downloader), depending on the state of \a checked.
    */
void pdSettingsDialog::on_checkBoxDALL_toggled(bool checked)
{
    if (checked == TRUE)
    {
        ui->checkBoxDBMP->setChecked(TRUE);
        ui->checkBoxDCanon->setChecked(TRUE);
        ui->checkBoxDDNG->setChecked(TRUE);
        ui->checkBoxDJPG->setChecked(TRUE);
        ui->checkBoxDNikon->setChecked(TRUE);
        ui->checkBoxDOlympus->setChecked(TRUE);
        ui->checkBoxDPentax->setChecked(TRUE);
        ui->checkBoxDPNG->setChecked(TRUE);
        ui->checkBoxDSigma->setChecked(TRUE);
        ui->checkBoxDSony->setChecked(TRUE);
        ui->checkBoxDTIFF->setChecked(TRUE);
    }
    else
    {
        ui->checkBoxDBMP->setChecked(FALSE);
        ui->checkBoxDCanon->setChecked(FALSE);
        ui->checkBoxDDNG->setChecked(FALSE);
        ui->checkBoxDJPG->setChecked(FALSE);
        ui->checkBoxDNikon->setChecked(FALSE);
        ui->checkBoxDOlympus->setChecked(FALSE);
        ui->checkBoxDPentax->setChecked(FALSE);
        ui->checkBoxDPNG->setChecked(FALSE);
        ui->checkBoxDSigma->setChecked(FALSE);
        ui->checkBoxDSony->setChecked(FALSE);
        ui->checkBoxDTIFF->setChecked(FALSE);
    }
}

/*!
    \fn pdSettingsDialog::on_checkBoxRALL_toggled(bool checked)

    Handles the "check all" checkbox (redistributor), depending on the state of \a checked.
    */
void pdSettingsDialog::on_checkBoxRALL_toggled(bool checked)
{
    if (checked == TRUE)
    {
        ui->checkBoxRBMP->setChecked(TRUE);
        ui->checkBoxRCanon->setChecked(TRUE);
        ui->checkBoxRDNG->setChecked(TRUE);
        ui->checkBoxRJPG->setChecked(TRUE);
        ui->checkBoxRNikon->setChecked(TRUE);
        ui->checkBoxROlympus->setChecked(TRUE);
        ui->checkBoxRPentax->setChecked(TRUE);
        ui->checkBoxRPNG->setChecked(TRUE);
        ui->checkBoxRSigma->setChecked(TRUE);
        ui->checkBoxRSony->setChecked(TRUE);
        ui->checkBoxRTIFF->setChecked(TRUE);
    }
    else
    {
        ui->checkBoxRBMP->setChecked(FALSE);
        ui->checkBoxRCanon->setChecked(FALSE);
        ui->checkBoxRDNG->setChecked(FALSE);
        ui->checkBoxRJPG->setChecked(FALSE);
        ui->checkBoxRNikon->setChecked(FALSE);
        ui->checkBoxROlympus->setChecked(FALSE);
        ui->checkBoxRPentax->setChecked(FALSE);
        ui->checkBoxRPNG->setChecked(FALSE);
        ui->checkBoxRSigma->setChecked(FALSE);
        ui->checkBoxRSony->setChecked(FALSE);
        ui->checkBoxRTIFF->setChecked(FALSE);
    }
}
