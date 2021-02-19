#ifndef CONNEXION_H
#define CONNEXION_H

#include <QDialog>

namespace Ui {
    class Connexion;
}

class Connexion : public QDialog {
    Q_OBJECT
public:
    Connexion(QWidget *parent = 0);
    ~Connexion();

private slots:
    void on_pushButtonAnnulConnexion_clicked();
    void on_pushButtonConnexion_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Connexion *ui;
};

#endif // CONNEXION_H
