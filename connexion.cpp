#include "connexion.h"
#include "ui_connexion.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTextCodec>
#include <QSqlQuery>
#include <QDebug>

Connexion::Connexion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connexion)
{
    ui->setupUi(this);
    //prise en compte de l'utf8
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    //masquer le mot de passe
    ui->lineEditMdp->setEchoMode(QLineEdit::Password);
}

Connexion::~Connexion()
{
    delete ui;
}

void Connexion::on_pushButtonConnexion_clicked()
{
    QString log = ui->lineEditLogin->text();
    QString mdp = ui->lineEditMdp->text();
    QString req = "select count(*) from admin where adminLogin = '"+log+"' and adminMdp='"+mdp+"' ";
    //qDebug()<<req;
    QSqlQuery connexion(req);
    connexion.first();
    if(connexion.value(0).toInt()==1)
    {
        accept();
    }
    else
    {
        ui->labelInfoConnexion->setText("Erreur. Vérifiez vos champs.");
        ui->lineEditMdp->clear();
    }

}


void Connexion::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Connexion::on_pushButtonAnnulConnexion_clicked()
{
    reject();
}
