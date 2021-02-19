#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void chargerVilles();
    void chargerZones();
    void chargerFamilles();
    void chargerRayons();
    void chargerLiaisons();


protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void on_pushButtonSupprLien_clicked();
    void on_pushButtonAjoutLien_clicked();
    void on_listWidgetLiaison_clicked(QModelIndex index);
    void on_comboBoxZoneOngLien_currentIndexChanged(int index);
    void on_comboBoxVilleOngLien_currentIndexChanged(int index);
    void on_pushButtonSupprProd_clicked();
    void on_pushButtonModifProd_clicked();
    void on_pushButtonAjoutProd_clicked();
    void on_pushButtonSupprRayon_clicked();
    void on_pushButtonModifRayon_clicked();
    void on_pushButtonAjoutRayon_clicked();
    void on_pushButtonSupprMagasin_clicked();
    void on_pushButtonModifMagasin_clicked();
    void on_pushButtonAjoutMagasin_clicked();
    void on_pushButtonSupprFamille_clicked();
    void on_pushButtonModifFamille_clicked();
    void on_comboBoxVilleOngZone_currentIndexChanged(int index);
    void on_pushButtonSupprZone_clicked();
    void on_pushButtonModifZone_clicked();
    void on_pushButtonAjoutZone_clicked();
    void on_pushButtonSupprVille_clicked();
    void on_pushButtonModifVille_clicked();
    void on_pushButtonAjoutVille_clicked();
    void on_listWidgetProd_clicked(QModelIndex index);
    void on_comboBoxRayonOngProd_currentIndexChanged(int index);
    void on_listWidgetRayons_clicked(QModelIndex index);
    void on_listWidgetMagasins_clicked(QModelIndex index);
    void on_listWidgetFamille_clicked(QModelIndex index);
    void on_comboBoxVilleOngMag_currentIndexChanged(QString ind);
    void on_listWidgetZone_clicked(QModelIndex index);
    void on_listWidgetVille_clicked(QModelIndex index);
};

#endif // MAINWINDOW_H
