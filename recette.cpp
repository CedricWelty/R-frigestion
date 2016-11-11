#include "recette.h"

Recette::Recette(QString nom, QString dureePrep, QList<Ingredient*> listeIng, QString etapesPrep, QString typeRecette, QString nomImage, QString cheminImage)
{
    this->nom = nom;
    this->dureePreparation = dureePrep;
    this->listeIngredients = listeIng;
    this->etapesPreparation = etapesPrep;
    this->typeRecette = typeRecette;
    this->nomImage = nomImage;
    this->cheminImage = cheminImage;
    affichee = true;
    actualiserRealisable();
}

void Recette::actualiserRealisable()
{
    this->realisable = checkRealisable();
}

bool Recette::checkRealisable()
{
    double sommeTemp;
    double quantitePossedee=0;
    int compteurOk =0;
    QString strTemp;
    QString nomTemp;
    QString uniteTemp;
    QString qteTemp;
    QFile fichier("listeIngredients.rfg");
    if(fichier.open(QIODevice::ReadOnly))
    {
        int j=0;
        QTextStream flux(&fichier);
        foreach(Ingredient* ing, listeIngredients)
        {
            quantitePossedee = 0;
            flux.seek(0);
            while(!flux.atEnd())
            {
                nomTemp="";
                uniteTemp="";
                qteTemp="";
                int i = 0;
                strTemp = flux.readLine();
                //Recupération du nom
                QChar c = strTemp[i];
                while(c != ';')
                {
                    nomTemp = nomTemp + c;
                    i++;
                    c = strTemp[i];
                }
                i++;
                 c = strTemp[i];
                //On passe la date
                while(c != ';')
                {
                    i++;
                    c = strTemp[i];
                }
                //Récupération de la quantité
                i++;
                c = strTemp[i];
                while(c != ';')
                {
                    qteTemp = qteTemp + c;
                    i++;
                    c = strTemp[i];
                }
                i++;
                c = strTemp[i];
                //Récupération de l'unité
                while(c != ';')
                {
                    uniteTemp = uniteTemp + c;
                    i++;
                    c = strTemp[i];
                }


                //Vérifier si le nom est le même
                if(QString::compare(ing->getNom(),nomTemp,Qt::CaseInsensitive)==0)
                {
                    sommeTemp = ajouterQuantites(quantitePossedee,ing->getUnite(),qteTemp.toDouble(),uniteTemp);
                    if(sommeTemp != -1)
                    {
                        quantitePossedee += sommeTemp;
                    }
                }
            }
            if(convertirQuantite(ing->getQuantite(),ing->getUnite()) > quantitePossedee)
            {
                fichier.close();
                return false;
            }
            else
            {
                compteurOk++;
            }
            j++;
        }
        if(compteurOk == listeIngredients.size())
        {
            fichier.close();
            return true;
        }
        else
        {
            fichier.close();
            return false;
        }
    }
    return false;
}

double Recette::convertirQuantite(double qte, QString unite)
{
    if(unite == "mL") return qte;
    else if(unite == "cL") return qte*10;
    else if(unite == "L") return qte*1000;
    else if(unite == "kg") return qte*1000;
    else if(unite == "g") return qte;
    else if(unite == "" || unite == " ") return qte;
    return qte;
}

/*
 *  Pour additionner les quantités en prenant en comte les unités
 */
double Recette::ajouterQuantites(double qte1, QString unite1, double qte2, QString unite2)
{
    if(((unite1 =="" || unite1 == " ") && (unite2=="" || unite2 == " ")) || (unite1 =="mL" && unite2=="mL") || (unite1 =="g" && unite2=="g")) return qte1 + qte2;
    else if (unite1 =="mL" && unite2=="cL") return qte1 + qte2*10;
    else if ((unite1 =="mL" && unite2=="L") || (unite1 =="g" && unite2=="kg")) return qte1 + qte2*1000;
    else if (unite1 =="cL" && unite2=="mL") return qte1*10 + qte2;
    else if (unite1 =="cL" && unite2=="L") return qte1*10 + qte2*1000;
    else if (unite1 =="cL" && unite2=="cL") return qte1*10 + qte2*10;
    else if ((unite1 =="L" && unite2=="L") || (unite1 =="kg" && unite2=="kg")) return qte1*1000 + qte2*1000;
    else if (unite1 =="L" && unite2=="cL") return qte1*1000 + qte2*10;
    else if ((unite1 =="L" && unite2=="mL") || (unite1 =="kg" && unite2=="g")) return qte1*1000 + qte2;
    else return -1;
}

bool Recette::isRealisable()
{
    return realisable;
}

bool Recette::aucunIngredient()
{
    return listeIngredients.isEmpty();
}

QString Recette::getNom()
{
    return nom;
}
QString Recette::getDureePreparation()
{
    return dureePreparation;
}

QString Recette::getNomImage()
{
    return nomImage;
}

QString Recette::getCheminImage()
{
    return cheminImage;
}

QList<Ingredient*> Recette::getListIngredients()
{
    return listeIngredients;
}

QString Recette::getEtapesPreparation()
{
    return etapesPreparation;
}

QString Recette::getTypeRecette()
{
    return typeRecette;
}

bool Recette::getAffichee()
{
    return affichee;
}

void Recette::setAffichee(bool affichee)
{
    this->affichee = affichee;
}
