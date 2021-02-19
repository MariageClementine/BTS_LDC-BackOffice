#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chargerVilles();
    chargerFamilles();
    chargerZones();
    chargerRayons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::chargerVilles()
{
    //reinitialisation des widgets
    ui->listWidgetVille->clear();
    ui->lineEditNomVille->clear();
    ui->comboBoxVilleOngMag->clear();
    ui->comboBoxVilleOngZone->clear();
    ui->comboBoxVilleOngLien->clear();

    ui->comboBoxVilleOngLien->addItem("...");
    ui->comboBoxVilleOngMag->addItem("...");
    ui->comboBoxVilleOngZone->addItem("...");

    //selection des villes
    QSqlQuery req("select villeLib from ville order by villeLib");
    while(req.next())
    {
        //chargement de la liste
        ui->listWidgetVille->addItem(req.value(0).toString());
        //chargement de la combobox dans longlet magasin
        ui->comboBoxVilleOngMag->addItem(req.value(0).toString());
        //chargement de la combobox dans longlet zone
        ui->comboBoxVilleOngZone->addItem(req.value(0).toString());
        //chargement de la combobox dans longlet lien
        ui->comboBoxVilleOngLien->addItem(req.value(0).toString());
    }
}

void MainWindow::chargerZones()
{
    //reinitilisation des widgets
    ui->listWidgetZone->clear();

    //selection des zones et villes correspondantes
    QSqlQuery req("select villeLib, ZoneLib from zone natural join ville order by villeLib, zoneLib");

    //affichage
    while(req.next())
    {
        ui->listWidgetZone->addItem(req.value(0).toString()+" , "+req.value(1).toString());
    }
}

void MainWindow::chargerRayons()
{
    //reinitialisation des widgets
    ui->listWidgetRayons->clear();
    ui->comboBoxRayonOngProd->clear();
    ui->comboBoxRayonOngLien->clear();
    ui->comboBoxRayonOngProd->addItem("...");
    ui->comboBoxRayonOngLien->addItem("...");

    //selection desrayons
    QSqlQuery req("select rayonLib from rayon order by rayonLib");
    while(req.next())
    {
        ui->listWidgetRayons->addItem(req.value(0).toString());
        ui->comboBoxRayonOngProd->addItem(req.value(0).toString());
        ui->comboBoxRayonOngLien->addItem(req.value(0).toString());
    }
}

void MainWindow::chargerFamilles()
{
    //reinitialisation des widgets
    ui->listWidgetFamille->clear();

    //selection des familles
    QSqlQuery req("select familleLibelle,familleCode from famille order by familleLibelle");
    //affichage
    while(req.next())
    {
        ui->listWidgetFamille->addItem(req.value(0).toString()+" , "+req.value(1).toString());
    }
}



void MainWindow::on_listWidgetVille_clicked(QModelIndex index)
{
    ui->lineEditNomVille->clear();
    QString nomVille = ui->listWidgetVille->currentItem()->text();
    ui->lineEditNomVille->setText(nomVille);
}

void MainWindow::on_listWidgetZone_clicked(QModelIndex index)
{
    QString ligne = ui->listWidgetZone->currentItem()->text();
    QStringList ligneSplit = ligne.split(" , ");
    QString ville= ligneSplit[0];
    QString zone = ligneSplit[1];
    ui->lineEditNomZone->setEnabled(true);

    ui->lineEditNomZone->setText(zone);
    int indexCombo = ui->comboBoxVilleOngZone->findText(ville);
    ui->comboBoxVilleOngZone->setCurrentIndex(indexCombo);
}

void MainWindow::on_comboBoxVilleOngMag_currentIndexChanged(QString ind)
{
    ui->lineEditNomMagasin->clear();
    ui->listWidgetMagasins->clear();
    ui->comboBoxZoneOngMag->clear();
    ui->comboBoxZoneOngMag->addItem("...");
    if(ui->comboBoxVilleOngMag->currentIndex()>0)
    {
        QString nomVille = ui->comboBoxVilleOngMag->currentText();
        //qDebug()<<nomVille;
        QSqlQuery req("select zoneLib, magasinLib from magasin natural join zone natural join ville where villeLib='"+nomVille+"'");
        while(req.next())
        {
            ui->listWidgetMagasins->addItem(req.value(0).toString()+" , "+req.value(1).toString());
            ui->comboBoxZoneOngMag->addItem(req.value(0).toString());
        }
        ui->comboBoxZoneOngMag->setEnabled(true);

    }
    else
    {
        ui->comboBoxZoneOngMag->setEnabled(false);
    }
}

void MainWindow::on_listWidgetFamille_clicked(QModelIndex index)
{
    QString ligne = ui->listWidgetFamille->currentItem()->text();
    QStringList ligneSplit = ligne.split(" , ");
    ui->lineEditNomFamille->setText(ligneSplit[0]);
    ui->lineEditCodeFamille->setText(ligneSplit[1]);
}

void MainWindow::on_listWidgetMagasins_clicked(QModelIndex index)
{
    QString ligne = ui->listWidgetMagasins->currentItem()->text();
    QStringList ligneSplit = ligne.split(" , ");
    int indexCombo = ui->comboBoxZoneOngMag->findText(ligneSplit[0]);
    ui->comboBoxZoneOngMag->setCurrentIndex(indexCombo);
    ui->lineEditNomMagasin->setText(ligneSplit[1]);
}

void MainWindow::on_listWidgetRayons_clicked(QModelIndex index)
{
    QString ligne = ui->listWidgetRayons->currentItem()->text();
    ui->lineEditNomRayon->setText(ligne);
}



void MainWindow::on_comboBoxRayonOngProd_currentIndexChanged(int index)
{
    ui->listWidgetProd->clear();
    ui->lineEditNomProd->clear();
    if(ui->comboBoxRayonOngProd->currentIndex()>0)
    {
        ui->lineEditNomProd->setEnabled(true);
        QString nomRayon = ui->comboBoxRayonOngProd->currentText();
        QSqlQuery req("select produitLib from produit natural join rayon where rayonLib='"+nomRayon+"' order by produitLib");
        while(req.next())
        {
            ui->listWidgetProd->addItem(req.value(0).toString());
        }
    }
    else
    {
        ui->lineEditNomProd->setEnabled(false);
    }
}

void MainWindow::on_listWidgetProd_clicked(QModelIndex index)
{
   QString nomProd = ui->listWidgetProd->currentItem()->text();
   ui->lineEditNomProd->setText(nomProd);
}

void MainWindow::on_pushButtonAjoutVille_clicked()
{
    //recup des infos
    QString nomVille = ui->lineEditNomVille->text();

    //verification
    if(nomVille!="")
    {
        QSqlQuery reqVerif("SELECT count(*) from ville where villeLib = '"+nomVille+"'");
        reqVerif.first();
        if(reqVerif.value(0).toInt()>0)
        {
            //message d'erreur
            ui->labelInfoVille->setText("Erreur, cette ville existe déjà.");
            ui->lineEditNomVille->clear();
        }
        else
        {
            QSqlQuery reqNum ("SELECT max(villeId)+1 from ville");
            reqNum.first();
            QString numVille= reqNum.value(0).toString();
            //qDebug()<<reqNum.value(0).toString();

            QSqlQuery reqInsert = ("INSERT INTO ville VALUES ("+numVille+",'"+nomVille+"')");
            reqInsert.exec();
            chargerVilles();
            ui->labelInfoVille->clear();
        }
    }
    else
    {
        //message d'erreur
        ui->labelInfoVille->setText("Erreur, votre champ est vide.");
    }
}

void MainWindow::on_pushButtonModifVille_clicked()
{

    QString oldNomVille = ui->listWidgetVille->currentItem()->text();
    QString newNomVille = ui->lineEditNomVille->text();
    //qDebug()<<oldNomVille+" "+newNomVille;
    QSqlQuery reqVerif("SELECT count(*) from ville where villeLib = '"+newNomVille+"'");
    reqVerif.first();
    if(reqVerif.value(0).toInt()>0)
    {
        //message d'erreur
        ui->labelInfoVille->setText("Erreur, cette ville existe déjà.");
    }
    else
    {
        QSqlQuery reqUpdate("Update ville set villeLib='"+newNomVille+"' where villeLib='"+oldNomVille+"'");
        reqUpdate.exec();
        chargerVilles();
        ui->labelInfoVille->clear();
    }
}

void MainWindow::on_pushButtonSupprVille_clicked()
{
    QString nomVille = ui->lineEditNomVille->text();
    QSqlQuery req("delete from ville where villeLib='"+nomVille+"'");
    req.exec();
    chargerVilles();
    ui->labelInfoVille->clear();
}

void MainWindow::on_pushButtonAjoutZone_clicked()
{
    //recup des infos
    QString nomZone = ui->lineEditNomZone->text();
    QString nomVille = ui->comboBoxVilleOngZone->currentText();

    //verification
    if(nomZone!="")
    {
        QSqlQuery reqVerif("SELECT count(*) from zone natural join ville where zoneLib='"+nomZone+"' and villeLib = '"+nomVille+"'");
        reqVerif.first();
        if(reqVerif.value(0).toInt()>0)
        {
            //message d'erreur
            ui->labelInfoZone->setText("Erreur, cette zone existe déjà.");
            ui->lineEditNomZone->clear();
        }
        else
        {
            QSqlQuery reqNum ("SELECT max(zoneId)+1 from zone");
            reqNum.first();
            QString numZone= reqNum.value(0).toString();
            //qDebug()<<reqNum.value(0).toString();

            QSqlQuery reqNumVille("select villeId from ville where villeLib='"+nomVille+"'");
            reqNumVille.first();
            qDebug()<<"select villeId from ville where villeLib='"+nomVille+"'";

            QSqlQuery reqInsert = ("INSERT INTO zone VALUES ("+numZone+",'"+nomZone+"',"+reqNumVille.value(0).toString()+")");
            reqInsert.exec();
            qDebug()<<"INSERT INTO zone VALUES ("+numZone+",'"+nomZone+"',"+reqNumVille.value(0).toString()+")";
            chargerZones();
            ui->labelInfoZone->clear();
        }
    }
    else
    {
        //message d'erreur
        ui->labelInfoZone->setText("Erreur, votre champ est vide.");
    }
}

void MainWindow::on_pushButtonModifZone_clicked()
{
    QString ligne =  ui->listWidgetZone->currentItem()->text();
    QStringList ligneSplit = ligne.split(" , ");

    QString oldNomZone = ligneSplit[1];
    QString newNomZone = ui->lineEditNomZone->text();
    //qDebug()<<oldNomZone+" "+newNomZone;
    QSqlQuery reqVerif("SELECT count(*) from zone where zoneLib = '"+newNomZone+"' and villeLib='"+ligneSplit[0]+"'");
    reqVerif.first();
    if(reqVerif.value(0).toInt()>0)
    {
        //message d'erreur
        ui->labelInfoZone->setText("Erreur, cette zone existe déjà.");
    }
    else
    {
        QSqlQuery reqNumVille("select villeId from ville where villeLib='"+ligneSplit[0]+"'");
        reqNumVille.first();
        qDebug()<<"select villeId from ville where villeLib='"+ligneSplit[0]+"'";

        QSqlQuery reqUpdate("Update zone set zoneLib='"+newNomZone+"' where zoneLib='"+oldNomZone+"' and villeId="+reqNumVille.value(0).toString());
        reqUpdate.exec();
        chargerZones();
        ui->labelInfoZone->clear();
    }
}

void MainWindow::on_pushButtonSupprZone_clicked()
{
    QStringList ligne = ui->listWidgetZone->currentItem()->text().split(" , ");

    QSqlQuery reqNumVille("select villeId from ville where villeLib='"+ligne[0]+"'");
    reqNumVille.first();
    QSqlQuery reqSuppr ("delete from zone where zoneLib='"+ligne[1]+"' and villeId="+reqNumVille.value(0).toString());
    chargerZones();
    ui->lineEditNomZone->clear();
    ui->comboBoxVilleOngZone->setCurrentIndex(0);
    ui->labelInfoZone->clear();
}

void MainWindow::on_comboBoxVilleOngZone_currentIndexChanged(int index)
{
    if(ui->comboBoxVilleOngZone->currentIndex()>0)
    {
        ui->lineEditNomZone->setEnabled(true);
    }
    else
    {
        ui->lineEditNomZone->setEnabled(false);
    }
}

void MainWindow::on_pushButtonModifFamille_clicked()
{
    ui->labelInfoFamille->clear();
    QStringList ligne = ui->listWidgetFamille->currentItem()->text().split(" , ");
    QString nom = ui->lineEditNomFamille->text();
    QString code = ui->lineEditCodeFamille->text();
    QSqlQuery reqVerif("select count(*) from famille where familleLibelle='"+nom+"' and familleCode='"+code+"'");
    reqVerif.first();
    if(reqVerif.value(0).toInt()>0)
    {
        ui->labelInfoFamille->setText("Erreur, cette famille existe déjà.");
    }
    else
    {
        QSqlQuery reqUpdate("update famille set familleLibelle='"+nom+"', familleCode='"+code+"' where familleCode='"+ligne[1]+"'");
        reqUpdate.exec();
        chargerFamilles();
    }
}

void MainWindow::on_pushButtonSupprFamille_clicked()
{
    ui->labelInfoFamille->clear();
    QString code = ui->lineEditCodeFamille->text();
    QSqlQuery req("delete from famille where familleCode='"+code+"'");
    req.exec();
    ui->lineEditCodeFamille->clear();
    ui->lineEditNomFamille->clear();
    chargerFamilles();
}

void MainWindow::on_pushButtonAjoutMagasin_clicked()
{
    QString nomVille = ui->comboBoxVilleOngMag->currentText();
    QString nomZone = ui->comboBoxZoneOngMag->currentText();
    QString nomMag = ui->lineEditNomMagasin->text();

    //verification
    if(nomMag!="" && ui->comboBoxZoneOngMag->currentIndex()>0)
    {
        QSqlQuery reqVerif("SELECT count(*) from magasin natural join zone natural join ville where villeLib = '"+nomVille+"' and zoneLib='"+nomZone+"' and magasinLib='"+nomMag+"'");
        reqVerif.first();
        if(reqVerif.value(0).toInt()>0)
        {
            //message d'erreur
            ui->labelInfoMag->setText("Erreur, ce magasin existe déjà.");
            ui->lineEditNomMagasin->clear();
        }
        else
        {
            ui->labelInfoMag->clear();
            QSqlQuery reqId("select max(magasinId)+1 from magasin");
            reqId.first();
            QSqlQuery reqZoneId ("select zoneId from zone natural join ville where zoneLib='"+nomZone+"' and villeLib='"+nomVille+"'");
            reqZoneId.first();

            QSqlQuery reqInsert ("insert into magasin values ("+reqId.value(0).toString()+",'"+nomMag+"',"+reqZoneId.value(0).toString()+")");
            reqInsert.exec();
            on_comboBoxVilleOngMag_currentIndexChanged("");
            ui->comboBoxVilleOngLien->setCurrentIndex(0);
            ui->comboBoxZoneOngLien->clear();
            ui->comboBoxZoneOngLien->addItem("...");
        }
    }
    else
    {
        ui->labelInfoMag->setText("Erreur, vérifiez vos champs.");
    }
}

void MainWindow::on_pushButtonModifMagasin_clicked()
{
    QString nomVille = ui->comboBoxVilleOngMag->currentText();
    QString newNomZone = ui->comboBoxZoneOngMag->currentText();
    QString newNomMag = ui->lineEditNomMagasin->text();
    QStringList ligne = ui->listWidgetMagasins->currentItem()->text().split(" , ");

    //verification
    if(newNomMag!="" && ui->comboBoxZoneOngMag->currentIndex()>0)
    {
        QSqlQuery reqVerif("SELECT count(*) from magasin natural join zone natural join ville where villeLib = '"+nomVille+"' and zoneLib='"+newNomZone+"' and magasinLib='"+newNomMag+"'");
        reqVerif.first();
        if(reqVerif.value(0).toInt()>0)
        {
            //message d'erreur
            ui->labelInfoMag->setText("Erreur, ce magasin existe déjà.");
        }
        else
        {
            ui->labelInfoMag->clear();
            QSqlQuery reqNewZoneId ("select zoneId from zone natural join ville where zoneLib='"+newNomZone+"' and villeLib='"+nomVille+"'");
            reqNewZoneId.first();

            QSqlQuery reqOldZoneId("select zoneId from zone natural join ville where zoneLib='"+ligne[0]+"' and villeLib='"+nomVille+"'");
            reqOldZoneId.first();

            QSqlQuery reqUpdate ("update magasin set magasinLib='"+newNomMag+"', zoneId='"+reqNewZoneId.value(0).toString()+"' where magasinLib='"+ligne[1]+"' and  zoneId='"+reqOldZoneId.value(0).toString()+"'");
            reqUpdate.exec();
            ui->comboBoxVilleOngLien->setCurrentIndex(0);
            ui->comboBoxZoneOngLien->clear();
            ui->comboBoxZoneOngLien->addItem("...");
            on_comboBoxVilleOngMag_currentIndexChanged("");
        }
    }
    else
    {
        ui->labelInfoMag->setText("Erreur, vérifiez vos champs.");
    }
}

void MainWindow::on_pushButtonSupprMagasin_clicked()
{
    ui->labelInfoMag->clear();
    QString zone = ui->comboBoxZoneOngMag->currentText();
    QString mag = ui->lineEditNomMagasin->text();
    QString ville = ui->comboBoxVilleOngMag->currentText();

    QSqlQuery Id ("select zoneId from zone natural join ville where zoneLib='"+zone+"' and villeLib='"+ville+"'");
    Id.first();

    QSqlQuery idMag("select magasinId from magasin where magasinLib='"+mag+"' and zoneId='"+Id.value(0).toString()+"'");
    idMag.first();

    QSqlQuery reqOrg("delete from organisation where magasinId="+idMag.value(0).toString());
    reqOrg.exec();

    QSqlQuery req("delete from magasin where magasinId="+idMag.value(0).toString());
    req.exec();
    ui->listWidgetLiaison->clear();
    ui->comboBoxVilleOngLien->setCurrentIndex(0);
    ui->comboBoxZoneOngLien->clear();
    ui->comboBoxZoneOngLien->addItem("...");
    on_comboBoxVilleOngMag_currentIndexChanged("");
}

void MainWindow::on_pushButtonAjoutRayon_clicked()
{
    ui->labelInfoRayon->clear();
    if(ui->lineEditNomRayon->text()!="")
    {
        QString nomRayon = ui->lineEditNomRayon->text();
        QSqlQuery reqVerif("select count(*) from rayon where rayonLib='"+nomRayon+"'");
        reqVerif.first();
        if(reqVerif.value(0).toInt()>0)
        {
            ui->labelInfoRayon->setText("Erreur, ce rayon existe déjà.");
            ui->labelInfoRayon->clear();
        }
        else
        {
            QSqlQuery reqId("select max(rayonId)+1 from rayon");
            reqId.first();

            QSqlQuery reqInsert("insert into rayon values ("+reqId.value(0).toString()+",'"+nomRayon+"')");
            reqInsert.exec();
            chargerRayons();
            ui->lineEditNomRayon->clear();
        }
    }
    else
    {
        ui->labelInfoRayon->setText("Erreur, votre champ est vide.");
    }
}

void MainWindow::on_pushButtonModifRayon_clicked()
{
    ui->labelInfoRayon->clear();
    if(ui->lineEditNomRayon->text()!="")
    {
        QString nomRayon = ui->lineEditNomRayon->text();
        QSqlQuery reqVerif("select count(*) from rayon where rayonLib='"+nomRayon+"'");
        reqVerif.first();
        if(reqVerif.value(0).toInt()>0)
        {
            ui->labelInfoRayon->setText("Erreur, ce rayon existe déjà.");
        }
        else
        {
            QString oldNomRayon = ui->listWidgetRayons->currentItem()->text();
            QSqlQuery reqUpdate("update rayon set rayonLib='"+nomRayon+"' where rayonLib='"+oldNomRayon+"'");
            reqUpdate.exec();
            chargerRayons();
            ui->lineEditNomRayon->clear();
        }
    }
    else
    {
        ui->labelInfoRayon->setText("Erreur, votre champ est vide.");
    }
}

void MainWindow::on_pushButtonSupprRayon_clicked()
{
    ui->labelInfoRayon->clear();
    QString ray = ui->listWidgetRayons->currentItem()->text();

    QSqlQuery idRay("select rayonId from rayon where rayonLib='"+ray+"'");
    idRay.first();

    QSqlQuery reqOrg("delete from organisation where rayonId="+idRay.value(0).toString());
    reqOrg.exec();
    QSqlQuery req("delete from rayon where rayonId="+idRay.value(0).toString());
    req.exec();
    chargerRayons();
    ui->lineEditNomRayon->clear();
    ui->listWidgetLiaison->clear();
    ui->comboBoxVilleOngLien->setCurrentIndex(0);
    ui->comboBoxZoneOngLien->clear();
    ui->comboBoxZoneOngLien->addItem("...");

}

void MainWindow::on_pushButtonAjoutProd_clicked()
{
    QString nomProd = ui->lineEditNomProd->text();

    if(nomProd != "" && ui->comboBoxRayonOngProd->currentIndex()>0)
    {
         QString nomRay = ui->comboBoxRayonOngProd->currentText();
         QSqlQuery reqVerif ("select count(*) from produit natural join rayon where produitLib='"+nomProd+"' and rayonLib='"+nomRay+"'");
         reqVerif.first();
         if(reqVerif.value(0).toInt()>0)
         {
             ui->labelInfoProd->setText("Erreur, ce produit existe déjà.");
             ui->lineEditNomProd->clear();
         }
         else
         {
             QSqlQuery reqId("select max(produitId)+1 from produit");
             reqId.first();

             QSqlQuery reqIdRay("select rayonId from rayon where rayonLib='"+nomRay+"'");
             reqIdRay.first();

             QSqlQuery reqInsert ("insert into produit values("+reqId.value(0).toString()+",'"+nomProd+"',"+reqIdRay.value(0).toString()+")");
             reqInsert.exec();
             //qDebug()<<"insert into produit values("+reqId.value(0).toString()+",'"+nomProd+"',"+reqIdRay.value(0).toString()+")";
             //qDebug()<<reqInsert.lastError();
             ui->lineEditNomProd->clear();
             on_comboBoxRayonOngProd_currentIndexChanged(0);
         }
    }
    else
    {
        ui->labelInfoProd->setText("Erreur, un champ est vide.");
    }
}

void MainWindow::on_pushButtonModifProd_clicked()
{
    QString nomProd = ui->lineEditNomProd->text();

    if(nomProd != "" && ui->comboBoxRayonOngProd->currentIndex()>0)
    {
         QString nomRay = ui->comboBoxRayonOngProd->currentText();
         QSqlQuery reqVerif ("select count(*) from produit natural join rayon where produitLib='"+nomProd+"' and rayonLib='"+nomRay+"'");
         reqVerif.first();
         if(reqVerif.value(0).toInt()>0)
         {
             ui->labelInfoProd->setText("Erreur, ce produit existe déjà.");
         }
         else
         {
             QString oldNomProd = ui->listWidgetProd->currentItem()->text();
             QSqlQuery reqIdRay("select rayonId from rayon where rayonLib='"+nomRay+"'");
             reqIdRay.first();

             QSqlQuery reqUpdate ("update produit set produitLib='"+nomProd+"' where produitLib='"+oldNomProd+"' and rayonId="+reqIdRay.value(0).toString());
             reqUpdate.exec();
             ui->lineEditNomProd->clear();
             on_comboBoxRayonOngProd_currentIndexChanged(0);
         }
    }
    else
    {
        ui->labelInfoProd->setText("Erreur, un champ est vide.");
    }
}

void MainWindow::on_pushButtonSupprProd_clicked()
{

    QString nomProd = ui->lineEditNomProd->text();
    QString nomRay = ui->comboBoxRayonOngProd->currentText();
    QSqlQuery reqIdRay("select rayonId from rayon where rayonLib='"+nomRay+"'");
    reqIdRay.first();
    QSqlQuery req("delete from produit where rayonId="+reqIdRay.value(0).toString()+" and produitLib='"+nomProd+"'");
    req.exec();
    ui->lineEditNomProd->clear();
    on_comboBoxRayonOngProd_currentIndexChanged(0);
}

void MainWindow::on_comboBoxVilleOngLien_currentIndexChanged(int index)
{
    if(ui->comboBoxVilleOngLien->currentIndex()>0)
    {
        ui->comboBoxZoneOngLien->clear();
        ui->comboBoxZoneOngLien->addItem("...");
        ui->comboBoxZoneOngLien->setEnabled(true);
        QString ville = ui->comboBoxVilleOngLien->currentText();
        QSqlQuery req ("select zoneLib from zone natural join ville where villeLib='"+ville+"'");
        while(req.next())
        {
            ui->comboBoxZoneOngLien->addItem(req.value(0).toString());
        }
    }
    else
    {
        ui->comboBoxMagasinOngLien->setCurrentIndex(0);
        ui->comboBoxRayonOngLien->setCurrentIndex(0);
        ui->comboBoxZoneOngLien->setCurrentIndex(0);
        ui->spinBoxOrdre->setValue(0);
        ui->comboBoxZoneOngLien->setEnabled(false);
        ui->comboBoxMagasinOngLien->setEnabled(false);
        ui->comboBoxRayonOngLien->setEnabled(false);
        ui->spinBoxOrdre->setEnabled(false);
        ui->pushButtonAjoutLien->setEnabled(false);
        ui->pushButtonSupprLien->setEnabled(false);
    }
}

void MainWindow::on_comboBoxZoneOngLien_currentIndexChanged(int index)
{
    ui->listWidgetLiaison->clear();
    ui->comboBoxMagasinOngLien->clear();
    ui->comboBoxMagasinOngLien->addItem("...");
    ui->comboBoxRayonOngLien->setCurrentIndex(0);
    ui->spinBoxOrdre->setValue(0);
    if(ui->comboBoxZoneOngLien->currentIndex()>0)
    {
        ui->comboBoxMagasinOngLien->setEnabled(true);
        ui->comboBoxRayonOngLien->setEnabled(true);
        ui->spinBoxOrdre->setEnabled(true);
        ui->pushButtonAjoutLien->setEnabled(true);
        QString nomVille = ui->comboBoxVilleOngLien->currentText();
        QString nomZone = ui->comboBoxZoneOngLien->currentText();

        QSqlQuery reqLiens("select rayonLib, magasinLib, organisationOrdre from rayon natural join organisation natural join magasin natural join zone natural join ville where villeLib='"+nomVille+"' and zoneLib='"+nomZone+"' order by magasinLib, organisationOrdre");
        //qDebug()<<"select rayonLib, magasinLib, organisationOrdre from rayon natural join organisation natural join magasin natural join zone natural join ville where villeLib='"+nomVille+"' and zoneLib='"+nomZone+"' order by magasinLib, organisationOrdre";
        while(reqLiens.next())
        {
            ui->listWidgetLiaison->addItem(reqLiens.value(1).toString()+" , "+reqLiens.value(0).toString()+" , "+reqLiens.value(2).toString());
        }

        QSqlQuery reqMags("select magasinLib from magasin natural join zone natural join ville where villeLib='"+nomVille+"' and zoneLib='"+nomZone+"' order by magasinLib");
        while(reqMags.next())
        {
            ui->comboBoxMagasinOngLien->addItem(reqMags.value(0).toString());
        }
    }
    else
    {
        ui->comboBoxMagasinOngLien->setEnabled(false);
        ui->comboBoxRayonOngLien->setEnabled(false);
        ui->spinBoxOrdre->setEnabled(false);
    }
}

void MainWindow::on_listWidgetLiaison_clicked(QModelIndex index)
{
    QStringList ligne = ui->listWidgetLiaison->currentItem()->text().split(" , ");
    int indexMag = ui->comboBoxMagasinOngLien->findText(ligne[0]);
    int indexRay = ui->comboBoxRayonOngLien->findText(ligne[1]);
    ui->comboBoxMagasinOngLien->setCurrentIndex(indexMag);
    ui->comboBoxRayonOngLien->setCurrentIndex(indexRay);
    ui->spinBoxOrdre->setValue(ligne[2].toInt());
    ui->pushButtonSupprLien->setEnabled(true);
}



void MainWindow::on_pushButtonAjoutLien_clicked()
{
    ui->labelInfoLien->setText("");
    if(ui->comboBoxMagasinOngLien->currentIndex()>0 && ui->comboBoxRayonOngLien->currentIndex()>0 && ui->spinBoxOrdre->value()>0)
    {
        QString ville = ui->comboBoxVilleOngLien->currentText();
        QString zone = ui->comboBoxZoneOngLien->currentText();
        QString magasin = ui->comboBoxMagasinOngLien->currentText();
        QSqlQuery recupMag("select magasinId from magasin natural join zone natural join ville where magasinLib='"+magasin+"' and zoneLib='"+zone+"' and villeLib='"+ville+"'");
        recupMag.first();

        QString rayon = ui->comboBoxRayonOngLien->currentText();
        QSqlQuery recupRayon("select rayonId from rayon where rayonLib='"+rayon+"'");
        recupRayon.first();

        int ordre = ui->spinBoxOrdre->value();
        QSqlQuery reqVerif ("select count(*) from organisation where magasinId="+recupMag.value(0).toString()+" and rayonId="+recupRayon.value(0).toString());
        reqVerif.first();
        if(reqVerif.value(0).toInt()>0)
        {
            ui->labelInfoLien->setText("Erreur, ce lien existe déjà.");
            ui->comboBoxMagasinOngLien->setCurrentIndex(0);
            ui->comboBoxRayonOngLien->setCurrentIndex(0);
            ui->spinBoxOrdre->setValue(0);
        }
        else
        {
            //on decale tout
            QSqlQuery reqMax("select max(organisationOrdre)+1 from organisation where magasinId="+recupMag.value(0).toString());
            reqMax.first();
            //si il y a des elements a decaler
            if(reqMax.value(0).toInt()!=ordre)
            {
                int oldOrdre;
                int compteur=2;
                int nvelOrdre;
                qDebug()<<"elements a decaler";
                //on decale la derniere valeur
                QSqlQuery recupDerniereValeur("select max(organisationOrdre) from organisation where magasinId ="+recupMag.value(0).toString());
                recupDerniereValeur.first();
                oldOrdre = recupDerniereValeur.value(0).toInt();
                nvelOrdre = oldOrdre+1;
                //qDebug()<<"update organisation set organisationOrdre="+QString::number(nvelOrdre)+" WHERE magasinId = "+recupMag.value(0).toString()+" and organisationOrdre="+QString::number(oldOrdre);
                QSqlQuery decalDerniereValeur("update organisation set organisationOrdre="+QString::number(nvelOrdre)+" WHERE magasinId = "+recupMag.value(0).toString()+" and organisationOrdre="+QString::number(oldOrdre));
                decalDerniereValeur.exec();

                //...et on décale les autres...
                //...sauf si il suffisait de décaler juste la dernière
                if(ordre!=oldOrdre)
                {
                    qDebug()<<"elements a decaler autres que le dernier";
                    do
                    {
                        QSqlQuery recupValSuivante("select max(organisationOrdre)-"+QString::number(compteur)+" from organisation where magasinId ="+recupMag.value(0).toString());
                        recupValSuivante.first();
                        oldOrdre = recupValSuivante.value(0).toInt();
                        nvelOrdre = oldOrdre+1;
                        qDebug()<<QString::number(oldOrdre)+" , "+QString::number(nvelOrdre);
                        QSqlQuery decalValSuivante("update organisation set organisationOrdre="+QString::number(nvelOrdre)+" WHERE magasinId = "+recupMag.value(0).toString()+" and organisationOrdre="+QString::number(oldOrdre));
                        qDebug()<<"update organisation set organisationOrdre="+QString::number(nvelOrdre)+" WHERE magasinId = "+recupMag.value(0).toString()+" and organisationOrdre="+QString::number(oldOrdre);
                        decalValSuivante.exec();
                        compteur++;
                    }
                    while(oldOrdre > ordre-1);
                }
            }
            QSqlQuery reqInsert("insert into organisation values("+recupMag.value(0).toString()+","+recupRayon.value(0).toString()+","+QString::number(ordre)+")");
           // qDebug()<<"insert into organisation values("+recupMag.value(0).toString()+","+recupRayon.value(0).toString()+","+QString::number(ordre)+")";
            reqInsert.exec();

            ui->spinBoxOrdre->setValue(0);
            on_comboBoxZoneOngLien_currentIndexChanged(0);
        }
    }
    else
    {
        ui->labelInfoLien->setText("Erreur, vérifiez vos champs");
    }
}

void MainWindow::on_pushButtonSupprLien_clicked()
{
    ui->labelInfoLien->setText("");
    QString ville = ui->comboBoxVilleOngLien->currentText();
    QString zone = ui->comboBoxZoneOngLien->currentText();
    QStringList ligne = ui->listWidgetLiaison->currentItem()->text().split(" , ");
    QSqlQuery recupMag("select magasinId from magasin natural join zone natural join ville where magasinLib='"+ligne[0]+"' and zoneLib='"+zone+"' and villeLib='"+ville+"'");
    recupMag.first();

    QSqlQuery recupRayon("select rayonId from rayon where rayonLib='"+ligne[1]+"'");
    recupRayon.first();

    int ordre = ui->spinBoxOrdre->value();

    QSqlQuery recupMax("select max(organisationOrdre) from organisation where magasinId="+recupMag.value(0).toString());
    recupMax.first();

    QSqlQuery req("delete from organisation where magasinId="+recupMag.value(0).toString()+" and rayonId="+recupRayon.value(0).toString()+" and organisationOrdre="+QString::number(ordre));
    req.first();

    //si la valeur supprimée n'etait pas la derniere
    if(recupMax.value(0).toInt() > ordre)
    {
        //on decale
        int nvelOrdre;
        while(nvelOrdre<recupMax.value(0).toInt())
        {
            nvelOrdre = ordre-1;
            QSqlQuery deplacer("update organisation set organisationOrdre= "+QString::number(nvelOrdre)+" where magasinId= "+recupMag.value(0).toString()+" and organisationOrdre="+QString::number(ordre));
            deplacer.exec();
            ordre++;
        }
    }
    ui->spinBoxOrdre->setValue(0);
    on_comboBoxZoneOngLien_currentIndexChanged(0);
    ui->pushButtonSupprLien->setEnabled(false);
}

