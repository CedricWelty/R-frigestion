#include "ingredient.h"
Ingredient::Ingredient(QString nom, int type, double quantite, QString unite, QDate datePeremption, QString cheminImage)
{
    this->nom = nom;
    this->type = type;
    this->datePeremption = datePeremption;
    this->unite = unite;
    this->quantite = quantite;
    this->cheminImage = cheminImage;
    int temp = quantite;
    increment = 1;
    while(temp > 10) {
        temp /= 10;
        increment *=10;
    }
    affiche = true;
}

Ingredient::Ingredient(QString strFichier)
{
   int i=0;
   QString strTemp;
   QChar c;

   c= strFichier[i];
   while(c != ';')
   {
        strTemp += c;
        i++;
        c = strFichier[i];
   }
   nom = strTemp;
   strTemp ="";
   i+=2;
   c= strFichier[i];
   while(c != ';')
   {
        strTemp += c;
        i++;
        c = strFichier[i];
   }
   quantite=strTemp.toDouble();
   strTemp ="";
   i++;
   c= strFichier[i];
   while(c != ';')
   {
        strTemp += c;
        i++;
        c = strFichier[i];
   }
   unite = strTemp;
   type =0;
   datePeremption=QDate();
   cheminImage="";
}

QString Ingredient::toStringDetail()
{
    QString str;
    if(quantite != 0)
     str += nom +" ("+QString::number(quantite)+unite+")";
    else
     str += nom;

    return str;
}

void Ingredient::setQuantite(double quantite) {
    this->quantite = quantite;
}

QString Ingredient::getNom()
{
    return nom;
}

QString Ingredient::getUnite()
{
    return unite;
}

double Ingredient::getQuantite()
{
    return quantite;
}

QDate Ingredient::getDate()
{
    return datePeremption;
}

QString Ingredient::getCheminImage()
{
    return cheminImage;
}

int Ingredient::getType()
{
    return type;
}

bool Ingredient::getAffiche()
{
    return affiche;
}

void Ingredient::setAffiche(bool affiche)
{
    this->affiche = affiche;
}

QString Ingredient::getTypeColor()
{
    static const QString colors[] = {"Images/autre.png", // Autre
                                     "Images/boisson.png", // Boisson
                                     "Images/legume.png", // Légume
                                     "Images/fruit.png", // Fruit
                                     "Images/viande.png", // Viande
                                     "Images/poisson.png", // Poisson
                                     "Images/fromage.png", // Fromage
                                     "Images/entree.png", //Entrée
                                     "Images/plat.png", //Plat
                                     "Images/dessert.png"}; // Dessert
    return colors[type];
}

QString Ingredient::getTypeColorLettre()
{
    static const QString colors[] = {"Images/autreLettre.png", // Autre
                                     "Images/boissonLettre.png", // Boisson
                                     "Images/legumeLettre.png", // Légume
                                     "Images/fruitLettre.png", // Fruit
                                     "Images/viandeLettre.png", // Viande
                                     "Images/poissonLettre.png", // Poisson
                                     "Images/fromageLettre.png", // Fromage
                                     "Images/entreeLettre.png", //Entrée
                                     "Images/platLettre.png", //Plat
                                     "Images/dessertLettre.png"}; // Dessert
    return colors[type];
}

int Ingredient::getIncrement()
{
    return increment;
}

Ingredient::~Ingredient()
{

}


