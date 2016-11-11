#include "mainwindow.h"
#include "ui_mainwindow.h"
#define NB_COLONNE_MAX 4
#define TAILLE_GROUPE_WIDGETS 0.2 // En fraction d'écran
#define TAILLE_GRILLE 0.7 // En fraction d'écran
#define LONGUEUR_COLONNE 200
#define HAUTEUR_LIGNE 200

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    screenWidth = QApplication::desktop()->screenGeometry().width();
    screenHeight = QApplication::desktop()->screenGeometry().height();
    this->showFullScreen();
    ui->widgets->setFixedWidth(screenWidth * TAILLE_GROUPE_WIDGETS);
    int tab = (screenWidth * TAILLE_GROUPE_WIDGETS *4)/2 - 14;
    tri = 1;
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 20px; width: "+ QString::number(tab) + "px; }");
    ui->tabWidget->setElideMode(Qt::ElideRight);

    ui->boutonAjoutIngredient->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    ui->boutonAjoutIngredient->setLayoutDirection(Qt::RightToLeft);
    connect(ui->boutonAjoutIngredient, SIGNAL(clicked(bool)), this, SLOT(ouvrirFenetreAjoutIngredient()));
    ui->boutonAjoutRecette->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    ui->boutonAjoutRecette->setLayoutDirection(Qt::RightToLeft);
    connect(ui->boutonAjoutRecette, SIGNAL(clicked(bool)), this, SLOT(ouvrirFenetreAjoutRecette()));
    connect(ui->trieBox, SIGNAL(currentIndexChanged(int)),this,SLOT(triage(int)));
    connect(ui->cb_afficher_type, SIGNAL(currentIndexChanged(int)), this, SLOT(actualiserAffichageTypeIngredient(int)));
    connect(ui->chb_faisable, SIGNAL(stateChanged(int)), this, SLOT(actualiserAffichageRecette()));
    connect(ui->cb_type, SIGNAL(currentTextChanged(QString)), this, SLOT(actualiserAffichageRecette()));
    fenAR = new FenetreAjoutRecette(this);
    fenAI = new FenetreAjoutIngredient(this);
    grilleIngredients = new QGridLayout();
    grilleRecettes = new QGridLayout();
    ui->verticalLayout->addLayout(grilleIngredients);
    ui->verticalLayout_2->addLayout(grilleRecettes);
    creerVignettesIngredientDemarrage();
    creerVignettesRecettesDemarrage();
    GestionDeFichiers::creerRecette("recette.rfg");
    creerPostit();
    connect(ui->textEdit_Postit, SIGNAL(textChanged()), this, SLOT(modifierContenuPostit()));
    updateHeure();
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateHeure()));
    timer->start(1000);
    ui->lcdNumberHeure->setFixedWidth(screenWidth * TAILLE_GROUPE_WIDGETS * 0.50);
    ui->lcdNumberHeure->setFixedHeight(30);
    ui->lcdNumberHeure->setSegmentStyle(QLCDNumber::Flat);
    ui->widgets->layout()->setAlignment(ui->lcdNumberHeure,Qt::AlignHCenter);
    ui->boutonAjoutIngredient->setShortcut(Qt::Key_A);
    ui->boutonAjoutRecette->setShortcut(Qt::Key_A);
    ui->timerMinuteur->setDisplayFormat("HH:mm:ss");
    connect(ui->buttonMinuteur, SIGNAL(pressed()), this, SLOT(lancerMinuteur()));

    tempMin = new QTimer();
    tempMinuteur = new QTimer();
    connect(tempMinuteur, SIGNAL(timeout()),this,SLOT(decrementMinuteur()));
    connect(tempMin, SIGNAL(timeout()),this,SLOT(finMinuteur()));

    ui->calendarWidget->setSelectedDate(QDate::currentDate());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ouvrirFenetreAjoutIngredient()
{
    fenAI = new FenetreAjoutIngredient(this);
    fenAI->show();
}

void MainWindow::ouvrirFenetreAjoutRecette()
{
    fenAR = new FenetreAjoutRecette(this);
    fenAR->show();
}

void MainWindow::ouvrirFenArModif(Recette* recette)
{
    fenAR = new FenetreAjoutRecette(this);
    fenAR->setContenu(recette);
    fenAR->show();
}

void MainWindow::ajoutIngredient()
{

    Ingredient* nouvelIngredient = fenAI->creerIngredient();
    if(nouvelIngredient->getNom() == "")
    {
            QMessageBox msgBox;
            msgBox.setWindowFlags(Qt::Popup);
            msgBox.setText("Erreur. Veuillez renseigner le champ 'Nom'.");
            msgBox.exec();
    }
    else {
        fenAI->close();
        fenAI = new FenetreAjoutIngredient(this);
        VignetteIngredient *newVignetteIngredient = new VignetteIngredient(screenWidth * TAILLE_GRILLE / NB_COLONNE_MAX,nouvelIngredient,this);
        grilleIngredients->addWidget(newVignetteIngredient, ingredients.size() / NB_COLONNE_MAX, ingredients.size() % NB_COLONNE_MAX);
        vignettesIngredients << newVignetteIngredient;
        ingredients << nouvelIngredient;
        switch(tri)
        {
            case 1:
                triAlphabetique();
                break;
            case 2:
                triDatePeremption();
                break;
            case 3:
                triCategorie();
        }
        updateVignettesRecettes();
    }
}
void MainWindow::modifRecette(Recette *recette)
{
    GestionDeFichiers::supprimerFichierRecette(recette);
    recettes.removeOne(recette);
    //Ajout de la recette
    ajoutRecette();
}

void MainWindow::ajoutRecette()
{
    //Ajout de la recette
    QString nomEntre =  fenAR->getNomEntre();
    if(GestionDeFichiers::recetteExisteDeja(nomEntre+".rfg") || !GestionDeFichiers::nomEstConforme(nomEntre))
    {
        QMessageBox msgBox;
        msgBox.setWindowFlags(Qt::Popup);
        msgBox.setText("Erreur. Le nom de la recette est non conforme ou la recette existe déjà.");
        fenAR->show();
        msgBox.exec();
    }
    else if(fenAR->pasDingredients())
    {
        QMessageBox msgBox;
        msgBox.setWindowFlags(Qt::Popup);
        msgBox.setText("Erreur. Veuillez renseigner au moins un ingrédient.");
        msgBox.exec();
    }
    else
    {
       Recette* nouvelleRecette = fenAR->creerRecette();
       fenAR->close();
       recettes << nouvelleRecette;
       actualiserAffichageRecette();
       updateVignettesRecettes();
    }
}

void MainWindow::creerVignettesIngredientDemarrage()
{
    QList<Ingredient*> listIng = GestionDeFichiers::listeIngredientsFichier();

    if (listIng.size() != 0)
    {
        foreach (Ingredient* ing, listIng)
        {
            VignetteIngredient *newVignetteIngredient = new VignetteIngredient(screenWidth * TAILLE_GRILLE / NB_COLONNE_MAX, ing, this);
            grilleIngredients->addWidget(newVignetteIngredient, ingredients.size() / NB_COLONNE_MAX, ingredients.size() % NB_COLONNE_MAX);
            vignettesIngredients << newVignetteIngredient;
            ingredients << ing;
        }
    }
    triAlphabetique();

}

void MainWindow::creerVignettesRecettesDemarrage()
{
    QDir rep("Recettes");
    QStringList filters;
    filters << "*.rfg";
    QStringList listeRep = rep.entryList(filters);
    if(listeRep.size()!=0)
    {
        foreach(QString chemin, listeRep)
        {
            Recette* nouvelleRecette = GestionDeFichiers::creerRecette(chemin);
            recettes << nouvelleRecette;
        }
        updateVignettesRecettes();
    }
}

bool MainWindow::supprimerVignetteIngredient(VignetteIngredient *vignette)
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::Popup);
    msgBox.setText("Vous êtes sur le point de supprimer définitivement cet ingrédient.\nConfirmer ?");
    msgBox.addButton(trUtf8("Oui"), QMessageBox::YesRole);
    QPushButton *noButton = msgBox.addButton(trUtf8("Non"), QMessageBox::NoRole);
    msgBox.setDefaultButton(noButton);
    if(msgBox.exec() == 0)
    {
        ingredients.removeOne(vignette->getIngredient());
        updateVignettesIngredients();
        updateVignettesRecettes();

    }
    else
    {
        return false;
    }
    reecrireFichier();
    return true;
}

void MainWindow::supprimerVignetteRecette(VignetteRecette *vignette)
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::Popup);
    msgBox.setText("Vous êtes sur le point de supprimer définitivement cette recette.\nConfirmer ?");
    msgBox.addButton(trUtf8("Oui"), QMessageBox::YesRole);
    QPushButton *noButton = msgBox.addButton(trUtf8("Non"), QMessageBox::NoRole);
    msgBox.setDefaultButton(noButton);
    if(msgBox.exec() == 0)
    {
        GestionDeFichiers::supprimerFichierRecette(vignette->getRecette());
        recettes.removeOne(vignette->getRecette());
        updateVignettesRecettes();
    }
}

void MainWindow::reecrireFichier()
{
    GestionDeFichiers::reecrireFichier(ingredients);
}
void MainWindow::creerPostit()
{
    QFontDatabase::addApplicationFont("Images/crackedJohnnie.ttf");

    QGridLayout * grillePostit = new QGridLayout();
    QFont fontPostit("Cracked Johnnie",16);
    QFile fichierPostit("postit.rfg");
    fichierPostit.open(QIODevice::ReadOnly);
    QString contenuPostit = fichierPostit.readAll();
    ui->label_Postit->setFixedSize(screenWidth * TAILLE_GROUPE_WIDGETS*0.97,screenWidth * TAILLE_GROUPE_WIDGETS*0.97);
    ui->label_Postit->setStyleSheet("QLabel#label_Postit{ border-image: url(Images/ZcPostIt.png) 0 0 0 0 stretch stretch; }");
    ui->label_Postit->setLayout(grillePostit);

    ui->textEdit_Postit->setFixedSize(screenWidth * TAILLE_GROUPE_WIDGETS *0.86,screenWidth * TAILLE_GROUPE_WIDGETS *0.53);
    ui->textEdit_Postit->setStyleSheet("background-color: transparent;");
    ui->textEdit_Postit->setFrameShape(QFrame::NoFrame);
    ui->textEdit_Postit->setFont(fontPostit);
    ui->textEdit_Postit->setText(contenuPostit);

    grillePostit->addWidget(ui->textEdit_Postit);
    fichierPostit.close();
}

void MainWindow::modifierContenuPostit()
{
    QString contenuPostit = ui->textEdit_Postit->toPlainText();
    QFile fichierPostit("postit.rfg");
    fichierPostit.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QTextStream fluxPostit(&fichierPostit);
    fluxPostit << contenuPostit;
    fichierPostit.close();
}

QList<Ingredient*> MainWindow::getIngredients()
{
    return this->ingredients;
}

void MainWindow::actualiserVignettesRecettes()
{
    foreach(VignetteRecette* vr, vignettesRecettes)
    {
        vr->actualiserAffichage();
    }
}

void MainWindow::updateHeure()
{
    ui->lcdNumberHeure->setDigitCount(8);
    ui->lcdNumberHeure->display(QTime::currentTime().toString());
    if(QTime::currentTime().toString() == "00:00:00")
    {
        foreach(VignetteIngredient *vignetteIngredient, vignettesIngredients)
        {
            vignetteIngredient->verifierPeremption();
        }
    }
}

bool MainWindow::nameLessThan(Ingredient *ing1,Ingredient *ing2)
{
    return ing1->getNom() < ing2->getNom();
}

bool MainWindow::dateLessThan(Ingredient *ing1,Ingredient *ing2)
{
    return ing1->getDate() < ing2->getDate();
}

bool MainWindow::categoryLessThan(Ingredient *ing1,Ingredient *ing2)
{
    return ing1->getType() < ing2->getType();
}

bool MainWindow::nameRecetteLessThan(Recette *rec1, Recette *rec2)
{
    return rec1->getNom() < rec2->getNom();
}

void MainWindow::triAlphabetique()
{
    tri = 1;
    qSort(ingredients.begin(), ingredients.end(), nameLessThan);
    updateVignettesIngredients();
}

void MainWindow::triDatePeremption()
{
    tri = 2;
    qSort(ingredients.begin(), ingredients.end(), dateLessThan);
    updateVignettesIngredients();
}

void MainWindow::triCategorie()
{
    tri = 3;
    qSort(ingredients.begin(), ingredients.end(), categoryLessThan);
    updateVignettesIngredients();
}

void MainWindow::triAlphabetiqueRecettes()
{
    qSort(recettes.begin(), recettes.end(), nameRecetteLessThan);
}

void MainWindow::triage(int i){
    switch(i){
        case 0 :
            triAlphabetique();
            break;
        case 1:
            triDatePeremption();
            break;
        case 2:
            triCategorie();
            break;
    }
}

void MainWindow::actualiserAffichageTypeIngredient(int type)
{
    foreach(Ingredient *ingredient, ingredients)
    {
        if(type == 0 || ingredient->getType() == (type - 1))
        {
            ingredient->setAffiche(true);
        }
        else
        {
            ingredient->setAffiche(false);
        }
    }
    if(type == 0 && ui->trieBox->itemText(2) == "") {
        ui->trieBox->addItem("Catégorie d'aliment");
    }
    else
    {
        ui->trieBox->setCurrentIndex(0);
        ui->trieBox->removeItem(2);
    }

    updateVignettesIngredients();
}

void MainWindow::actualiserAffichageRecette()
{
    foreach(Recette *recette, recettes)
    {
        if((ui->cb_type->currentText() == "Tout" || recette->getTypeRecette() == ui->cb_type->currentText()))
        {
            if(ui->chb_faisable->isChecked() && !recette->isRealisable()) {
                recette->setAffichee(false);
            }
            else
            {
                recette->setAffichee(true);
            }
        }
        else
        {
            recette->setAffichee(false);
        }
    }
    updateVignettesRecettes();
}

void MainWindow::updateVignettesIngredients()
{
    foreach(VignetteIngredient *vignette, vignettesIngredients) vignette->deleteLater();
    vignettesIngredients.clear();
    delete grilleIngredients;
    grilleIngredients = new QGridLayout();
    ui->verticalLayout->addLayout(grilleIngredients);

    foreach(Ingredient *ingredient, ingredients)
    {
        if(ingredient->getAffiche())
        {
            VignetteIngredient *newVignetteIngredient = new VignetteIngredient(screenWidth * TAILLE_GRILLE / NB_COLONNE_MAX, ingredient, this);
            grilleIngredients->addWidget(newVignetteIngredient, vignettesIngredients.size() / NB_COLONNE_MAX, vignettesIngredients.size() % NB_COLONNE_MAX);
            vignettesIngredients << newVignetteIngredient;
        }
    }
}

void MainWindow::updateVignettesRecettes()
{
    triAlphabetiqueRecettes();
    foreach(VignetteRecette *vignette, vignettesRecettes) vignette->deleteLater();
    vignettesRecettes.clear();
    delete grilleRecettes;
    grilleRecettes = new QGridLayout();
    ui->verticalLayout_2->addLayout(grilleRecettes);
    foreach(Recette *recette, recettes)
    {
        if(recette->getAffichee())
        {
            VignetteRecette *newVignetteRecette = new VignetteRecette(screenWidth * TAILLE_GRILLE / NB_COLONNE_MAX, recette, this);
            grilleRecettes->addWidget(newVignetteRecette, vignettesRecettes.size() / NB_COLONNE_MAX, vignettesRecettes.size() % NB_COLONNE_MAX);
            vignettesRecettes << newVignetteRecette;
        }
    }
}

void MainWindow::lancerMinuteur()
{
    if(!(tempMin->isActive()) && ui->timerMinuteur->time().hour() + ui->timerMinuteur->time().minute() + ui->timerMinuteur->time().second() != 0)
    {
        ui->timerMinuteur->setDisabled(true);
        tempMinuteur->start(1000);
        tempMin->start(ui->timerMinuteur->time().hour() * 3600000 + ui->timerMinuteur->time().minute() * 60000 + ui->timerMinuteur->time().second() * 1000);
        ui->buttonMinuteur->setText("Arreter");
    }else if(tempMinuteur->isActive()){
        tempMin->stop();
        tempMinuteur->stop();
        ui->timerMinuteur->setTime(QTime(0,0,0));
        ui->timerMinuteur->setDisabled(false);
        ui->buttonMinuteur->setText("Lancer");
    }
}

void MainWindow::finMinuteur()
{
    tempMin->stop();
    tempMinuteur->stop();
    ui->timerMinuteur->setDisabled(false);
    ui->timerMinuteur->setTime(QTime(0,0,0));
    ui->buttonMinuteur->setText("Lancer");
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("Minuteur.wav"));
    player->setVolume(100);
    player->play();
}

void MainWindow::decrementMinuteur()
{
    ui->timerMinuteur->setTime(ui->timerMinuteur->time().addSecs(-1));
}
