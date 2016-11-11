#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QDate>
#include <QPushButton>
#include <QPixmap>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>

class Ingredient //: public QWidget
{
public:
    explicit Ingredient(QString nom, int type, double quantite, QString unite, QDate datePeremption, QString cheminImage);
    Ingredient(QString strFichier);
    ~Ingredient();
    void setQuantite(double);
    QString getNom();
    double getQuantite();
    QString getUnite();
    QDate getDate();
    QString getCheminImage();
    int getType();
    QString getTypeColor();
    QString getTypeColorLettre();
    QString toStringDetail();
    bool getAffiche();
    int getIncrement();
    void setAffiche(bool affiche);


private:
    int increment;
    double quantite;
    QString nom;
    QString unite;
    QDate datePeremption;
    QString cheminImage;
    int type;
    bool affiche;
};

#endif // INGREDIENT_H

