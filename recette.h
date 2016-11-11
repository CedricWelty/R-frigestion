#ifndef RECETTE_H
#define RECETTE_H

#include <QObject>
#include <ingredient.h>
#include <QFile>
#include <QTextStream>
class Recette
{
public:
    explicit Recette(QString nom, QString dureePrep, QList<Ingredient*> listeIng, QString etapesPrep, QString typeRecette, QString nomImage, QString cheminImage);
    QString getNom();
    QString getDureePreparation();
    QString getCheminImage();
    QString getNomImage();
    QString getEtapesPreparation();
    bool isRealisable();
    void creerFichierRecette();
    QList<Ingredient *> getListIngredients();
    QString getTypeRecette();
    bool aucunIngredient();
    void actualiserRealisable();
    void setAffichee(bool affichee);
    bool getAffichee();
    ~Recette();
private :
    bool checkRealisable();
    bool affichee;
    QString nom;
    QString dureePreparation;
    QList<Ingredient*> listeIngredients;
    QString etapesPreparation;
    QString typeRecette;
    QString cheminImage;
    QString nomImage;
    bool realisable;
    double ajouterQuantites(double qte1, QString unite1, double qte2, QString unite2);
    double convertirQuantite(double qte, QString unite);
};

#endif // RECETTE_H
