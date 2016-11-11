#include "gestiondefichiers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <QFile>
#include <recette.h>

using namespace std;

GestionDeFichiers::GestionDeFichiers()
{

}
/*
 * Ajoute un ingredient au fichier listeIngredients.rfg si ing est renseigné
 * Ajoute un fichier portant le nom de la recette si recette est renseigné,
 *  le contenu de ce fichier correspond à la recette passée en paramètre.
 *
 * Renvoie 0 si l'opération a réussi, 1 sinon.
 */
int GestionDeFichiers::ajoutFichier(Recette* recette, Ingredient* ing)
{
    //Tester si le fichier existe déja
    //ControleEspace et MIN de Nom
    if(recette == NULL && ing !=NULL)
    {
        QFile fichier("listeIngredients.rfg");
        if(fichier.open(QIODevice::ReadWrite | QIODevice::Append))
        {
            QTextStream flux(&fichier);
            flux << creerLigneIngredient(ing);
            fichier.close();
            return 0;
        }
        else
        {
            cout << "Erreur d'ouverture de fichier" << endl;
        }
    }
    //Pour Créer une recette
    else if(recette != NULL && ing == NULL)
    {
        QDir dossier;
        dossier.mkpath("Recettes");
        QString nomFichier = "Recettes/"+recette->getNom()+".rfg";

        QFile fichier(nomFichier);
        if(fichier.open(QIODevice::ReadWrite | QIODevice::Append))
        {
            //fichier.write();
            QTextStream flux(&fichier);
            flux << creerStringRecette(recette);
            fichier.close();
            return 0;
        }
        else
        {
            cout << "Erreur d'ouverture de fichier" << endl;
            return 1;
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

QString GestionDeFichiers::creerLigneIngredient(Ingredient *ing)
{
    QString ligne(ing->getNom());
    ligne +=  ";" + ing->getDate().toString("dd.MM.yyyy") + ";" + QString::number(ing->getQuantite()) + ";" + ing->getUnite() + ";" + QString::number(ing->getType()) +";" + ing->getCheminImage()+";\n" ;
    return ligne;
}

QString GestionDeFichiers::creerStringRecette(Recette* recette)
{
    //Vérifier que le fichier n'existe pas déjà
    QString ligne(recette->getNom());
    ligne += "\n"+ recette->getDureePreparation();
    ligne += "\n"+ recette->getTypeRecette()+"\n;\n";

    foreach(Ingredient* ing, recette->getListIngredients())
    {
        ligne += creerLigneIngredient(ing)+"\n";
    }

    ligne += ";\n" + recette->getEtapesPreparation();
    ligne += "\n;\n" + recette->getNomImage()+"\n";
    ligne += recette->getCheminImage()+"\n" ;

    return ligne;
}

Ingredient* GestionDeFichiers::creerIngredient(QString ligneFichier)
{
    QString nom;
    QString dateTab[3];
    //QDate datePeremption;
    QString quantite;
    QString unite;
    QString type;
    QString cheminImage;
    int jour, mois, annee;
    int i=0;

    //Recupération du nom
    QChar c = ligneFichier[i];
    while(c != ';')
    {
        nom = nom + c;
        i++;
        c = ligneFichier[i];

    }

    i++;
    //Récupération de la date
    for(int j=0; j<2; ++j)
    {
        c = ligneFichier[i];

        while(c != '.')
        {
            dateTab[j] = dateTab[j] + c;
            i++;
            c = ligneFichier[i];
        }
        i++;
    }
    c = ligneFichier[i];
    while(c != ';')
    {
        dateTab[2] = dateTab[2] + c;
        i++;
        c = ligneFichier[i];
    }
    jour = dateTab[0].toInt(); //je sais c'est pas opti mais
    mois = dateTab[1].toInt(); //sinon ça marche pas . . .
    annee = dateTab[2].toInt();

    QDate datePeremption(annee,mois,jour);

    //Récupération de la quantité
    i++;
    c = ligneFichier[i];
    while(c != ';')
    {
        quantite = quantite + c;
        i++;
        c = ligneFichier[i];
    }

    i++;
    c = ligneFichier[i];
    //Récupération de l'unité
    while(c != ';')
    {
        unite = unite + c;
        i++;
        c = ligneFichier[i];
    }

    i++;
    c = ligneFichier[i];
    //Récupération du type
    while(c != ';')
    {
        type = type + c;
        i++;
        c = ligneFichier[i];
    }
    i++;
    c = ligneFichier[i];

    //Récupération du chemin de l'image
    while(c != ';')
    {
        cheminImage = cheminImage + c;
        i++;
        c = ligneFichier[i];
    }

    Ingredient* nouvelIngredient = new Ingredient(nom,type.toInt(),quantite.toDouble(),unite,datePeremption,cheminImage);
    return nouvelIngredient;
}

bool GestionDeFichiers::recetteExisteDeja(QString nomFichier)
{

    QDir rep("Recettes");
    QStringList filters;
    filters << "*.rfg";
    QStringList listeRep = rep.entryList(filters);

    foreach(QString s, listeRep)
    {
        if(s == nomFichier)
            return true;
    }
    return false;
}

Recette* GestionDeFichiers::creerRecette(QString nomFichier)
{
    QString nom;
    QString dureePrep;
    QList<Ingredient*> listIng;
    QString etapesPrep;
    QString type;
    QString nomImage;
    QString cheminImage;
    QString strTemp;

    QFile fichier("Recettes/"+nomFichier);
    if(fichier.open(QIODevice::ReadOnly))
    {
        QTextStream flux(&fichier);
        while(!flux.atEnd())
        {
            nom = flux.readLine();
            dureePrep = flux.readLine();
            type = flux.readLine();
            flux.readLine();
            strTemp = flux.readLine();
            while(strTemp != ";")
            {
                if(strTemp != "" )
                    listIng << new Ingredient(strTemp);
                strTemp = flux.readLine();
            }
            strTemp = flux.readLine();
            while(strTemp != ";")
            {
                etapesPrep += strTemp+"\n";
                strTemp = flux.readLine();
            }
            nomImage = flux.readLine();
            cheminImage = flux.readLine();
        }
    }
    Recette* recette = new Recette(nom,dureePrep,listIng,etapesPrep,type,nomImage,cheminImage);
    return recette;
}

QList<Ingredient*> GestionDeFichiers::listeIngredientsFichier()
{
    QList<Ingredient*> listIng;
    QFile fichier("listeIngredients.rfg");
    if(fichier.open(QIODevice::ReadOnly))
    {
        QTextStream flux(&fichier);
        while(!flux.atEnd())
        {
            QString ligne = flux.readLine();
            listIng << creerIngredient(ligne);
        }
        fichier.close();
    }
     return listIng;
}

void GestionDeFichiers::reecrireFichier(QList<Ingredient*> listIng)
{
    QFile fichier("listeIngredients.rfg");
    if(fichier.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream flux(&fichier);
        foreach(Ingredient* ing , listIng)
        {
            flux << creerLigneIngredient(ing);
        }
    }
        fichier.close();
}


int GestionDeFichiers::modifierQuantiteIngredient(Ingredient* ing)
{
    QFile fichier("listeIngredients.rfg");
    QString contenuTemp;
    QTextStream flux(&fichier);
    QString ligneIng = creerLigneIngredient(ing);
    ligneIng = ligneIng.left(ligneIng.size()-1);

    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!flux.atEnd())
        {
            QString ligne = flux.readLine();
            if(ligne.compare(ligneIng)== -1)
            {
                contenuTemp += ligneIng +"\n";
            }
            else
            {
                contenuTemp += ligne +"\n";
            }
        }
        fichier.close();
        if(fichier.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
           flux << contenuTemp;
           fichier.close();
           return 0;
        }

        return 1;
    }
    return 0;
}
bool GestionDeFichiers::supprimerFichierRecette(Recette* recette)
{
    QString nomFichier = "Recettes/" + recette->getNom() + ".rfg";
    return QFile::remove(nomFichier);
}

bool GestionDeFichiers::nomEstConforme(QString nomFichier)
{
    if(nomFichier == "" || nomFichier.contains("<") || nomFichier.contains(">") || nomFichier.contains(":") || nomFichier.contains("\\") || nomFichier.contains("/") || nomFichier.contains("|") || nomFichier.contains("?") || nomFichier.contains("*"))
    {
        return false;
    }
    else {
        return true;
    }
}
