#include "fenetreajoutrecette.h"
#include "ui_fenetreajoutrecette.h"
#include <mainwindow.h>

static const char color[] = "#FF4500";

FenetreAjoutRecette::FenetreAjoutRecette(MainWindow *parent):
    QDialog(parent),
    ui(new Ui::FenetreAjoutRecette)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::SplashScreen);
    this->setModal(true);
    this->setWindowTitle("Ajouter une recette");
    window = parent;

    ui->label1->setStyleSheet("color:" + QString(color) + ";font : bold;");
    ui->label2->setStyleSheet("color:"+ QString(color)+";font : bold;");
    ui->label3->setStyleSheet("color:"+QString(color)+";font : bold;");
    ui->label4->setStyleSheet("color:"+QString(color)+";font : bold;");
    ui->label5->setStyleSheet("color:"+QString(color)+";font : bold;");
    ui->label6->setStyleSheet("color:"+QString(color)+";font : bold;");

    le_list << ui->le_ing1 <<  ui->le_ing2;
    cb_list << ui->cb_ing1 << ui->cb_ing2;
    sb_list << ui->sb_ing1 << ui->sb_ing2;

    QDir *image = new QDir("Images/Contenu");
    image->setFilter(QDir::Files);
    image->setSorting(QDir::Name);

    QFileInfoList list = image->entryInfoList();
    QFileInfo fileInfo;
    int i=0;
    do
    {
        fileInfo = list.at(i);
        i++;
    } while(fileInfo.fileName().split(".").at(0)!="Autre");
    ui->cb_image->addItem(fileInfo.fileName().split(".").at(0),fileInfo.filePath());
    for(i=0; i< list.size(); ++i){
        fileInfo = list.at(i);
        if(fileInfo.fileName().split(".").at(0)!="Autre")
        {
            ui->cb_image->addItem(fileInfo.fileName().split(".").at(0),fileInfo.filePath());
        }
    }

    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(ajoutModifRecette()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->tbAjouterIng,SIGNAL(clicked()),this,SLOT(ajouterLigneTableIng()));
}

Recette* FenetreAjoutRecette::creerRecette()
{
    Recette* recetteTemp = new Recette(ui->le_nom->text(),ui->te_duree->text(),creerListeIngredients(),ui->te_etapes->toPlainText(),ui->cb_type->currentText(),ui->cb_image->currentText(), ui->cb_image->currentData().toString());
    GestionDeFichiers::ajoutFichier(recetteTemp,NULL);
    return recetteTemp;
}

void FenetreAjoutRecette::ajoutModifRecette()
{
    if(ui->okButton->text() == "Modifier")
    {
        QMessageBox msgBox;
        msgBox.addButton(trUtf8("Oui"), QMessageBox::NoRole);
        QPushButton *noButton = msgBox.addButton(trUtf8("Non"), QMessageBox::NoRole);
        msgBox.setDefaultButton(noButton);
        msgBox.setWindowFlags(Qt::Popup);

        msgBox.setText("Vous êtes sur le point de modifier cette recette.\nConfirmer ?");
        if(msgBox.exec() == 0)
        {
            if(pasDingredients())
            {
                QMessageBox msgBox;
                msgBox.setWindowFlags(Qt::Popup);
                msgBox.setText("Erreur. Veuillez renseigner au moins un ingrédient.");
                msgBox.exec();
            }
            else
            {
                window -> modifRecette(recette);
            }
        }
    }
    else
    {
        window->ajoutRecette();
    }
}

QString FenetreAjoutRecette::getFenAREtat()
{
    return ui->okButton->text();
}

void FenetreAjoutRecette::setContenu(Recette * recette)
{
    ui->le_nom->setText(recette->getNom());
    this->recette = recette;
    ui->cb_type->setCurrentText(recette->getTypeRecette());
    ui->te_etapes->setText(recette->getEtapesPreparation());
    //ui->te_duree->setText(recette->getDureePreparation());
    ui->te_duree->setTime(QTime(recette->getDureePreparation().left(2).toInt(), recette->getDureePreparation().right(2).toInt()));
    ui->cb_image->setCurrentText(recette->getNomImage());

    int nbIngredients = recette->getListIngredients().size();
    if(nbIngredients>2)
    {
        for(int i=0;i<nbIngredients-2;i++)
        {
            ajouterLigneTableIng();
        }
    }
    for(int i=0;i<nbIngredients;i++)
    {
        Ingredient* ingTemp = recette->getListIngredients().at(i);
        le_list.at(i)->setText(ingTemp->getNom());
        cb_list.at(i)->setCurrentText(ingTemp->getUnite());
        sb_list.at(i)->setValue(ingTemp->getQuantite());
    }

    ui->okButton->setText("Modifier");
}

void FenetreAjoutRecette::ajouterLigneTableIng()
{
    QLineEdit* le = new QLineEdit();
    le->setMinimumWidth(175);
    QDoubleSpinBox* sb = new QDoubleSpinBox();
    QComboBox* cb = new QComboBox();
    cb->addItem(QIcon()," ",QVariant());
    cb->addItem(QIcon(),"mL",QVariant());
    cb->addItem(QIcon(),"cL",QVariant());
    cb->addItem(QIcon(),"L",QVariant());
    cb->addItem(QIcon(),"g",QVariant());
    cb->addItem(QIcon(),"kg",QVariant());
    cb->setMinimumWidth(45);

    sb->setMaximum(5000);
    sb->setMinimum(0.01);
    sb->setSingleStep(1);
    sb->setValue(1);

    ui->grilleIngredients->addWidget(le,ui->grilleIngredients->rowCount()+1,0,Qt::AlignLeft);
    ui->grilleIngredients->addWidget(sb,ui->grilleIngredients->rowCount()-1,1,Qt::AlignLeft);
    ui->grilleIngredients->addWidget(cb,ui->grilleIngredients->rowCount()-1,2,Qt::AlignCenter);

    le_list << le;
    cb_list << cb;
    sb_list << sb;
}

QList<Ingredient*> FenetreAjoutRecette::creerListeIngredients()
{
    QList<Ingredient*> liste;

    int nbLignes = le_list.size();
    for(int i=0; i<nbLignes; i++)
    {
        if(le_list.at(i)->text() != "" && le_list.at(i)->text() != NULL )
        {
            Ingredient* ingTemp = new Ingredient(le_list.at(i)->text(),0,sb_list.at(i)->value(),cb_list.at(i)->currentText(),QDate(),"");
            liste << ingTemp;
        }
    }
    return liste;
}

bool FenetreAjoutRecette::pasDingredients()
{
    foreach(QLineEdit* le, le_list)
    {
        if(le->text()!= "" && le->text() != NULL)
        {
            return false;
        }
    }
    return true;
}

QString FenetreAjoutRecette::getNomEntre()
{
    return ui->le_nom->text();
}

FenetreAjoutRecette::~FenetreAjoutRecette()
{
    delete ui;
}


