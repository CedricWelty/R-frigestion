#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QObject>
#include <QLabel>
#include <QList>
#include <QDialog>
#include <QPushButton>
#include <QFontDatabase>
#include <QStringListModel>
#include <QCalendarWidget>
#include <QTimer>
#include <QMediaPlayer>
#include <ingredient.h>
#include <recette.h>
#include <fenetreajoutingredient.h>
#include <fenetreajoutrecette.h>
#include <vignetteingredient.h>
#include <QMessageBox>
#include <gestiondefichiers.h>
#include <QDir>
#include <vignetterecette.h>
#include <QLCDNumber>
#include <QTimeEdit>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QList<Ingredient*> getIngredients();
    bool supprimerVignetteIngredient(VignetteIngredient* vi);
    void supprimerVignetteRecette(VignetteRecette * vr);
    void reecrireFichier();


private:
    Ui::MainWindow *ui;
    QList<Recette*> recettes;
    QList<Ingredient*> ingredients;
    QList<VignetteIngredient*> vignettesIngredients;
    QList<VignetteRecette*> vignettesRecettes;
    int screenWidth;
    int screenHeight;
    int tri;
    FenetreAjoutIngredient* fenAI;
    FenetreAjoutRecette* fenAR;
    QGridLayout *grilleIngredients;
    QGridLayout *grilleRecettes;
    QTimer* tempMinuteur;
    QTimer * tempMin;

    void updateVignettesIngredients();
    void updateVignettesRecettes();
    void creerVignettesIngredientDemarrage();
    void creerVignettesRecettesDemarrage();
    void creerPostit();
    void creerListeIngredientDateLimite();
    void updateMinuteur();
    void triAlphabetiqueRecettes();
    static bool nameLessThan(Ingredient *ing1, Ingredient *ing2);
    static bool categoryLessThan(Ingredient *ing1, Ingredient *ing2);
    static bool dateLessThan(Ingredient *ing1, Ingredient *ing2);
    static bool nameRecetteLessThan(Recette *rec1, Recette *rec2);


public slots:
    void ouvrirFenetreAjoutIngredient();
    void ajoutIngredient();
    void ouvrirFenetreAjoutRecette();
    void ajoutRecette();
    void modifRecette(Recette *recette);
    void ouvrirFenArModif(Recette* recette);
    void modifierContenuPostit();
    void updateHeure();
    void triage(int);
    void triAlphabetique();
    void triDatePeremption();
    void triCategorie();
    void actualiserVignettesRecettes();
    void actualiserAffichageTypeIngredient(int);
    void actualiserAffichageRecette();
    void finMinuteur();
    void lancerMinuteur();
    void decrementMinuteur();
};

#endif // MAINWINDOW_H
