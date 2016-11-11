#include "fenetreajoutingredient.h"
#include "ui_fenetreajoutingredient.h"
#include <gestiondefichiers.h>

static const char color[] = "#FF4500";

FenetreAjoutIngredient::FenetreAjoutIngredient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FenetreAjoutIngredient)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::SplashScreen);
    this->setModal(true);
    this->setWindowTitle("Ajouter un ingrédient");
    ui->de_date->setDateTime(QDateTime::currentDateTime().addDays(3));
    QDir *image = new QDir("Images/Contenu");
    image->setFilter(QDir::Files);
    image->setSorting(QDir::Name);
    ui->label1->setStyleSheet("color:" + QString(color) + ";font : bold;");
    ui->label2->setStyleSheet("color:"+ QString(color)+";font : bold;");
    ui->label3->setStyleSheet("color:"+QString(color)+";font : bold;");
    ui->label4->setStyleSheet("color:"+QString(color)+";font : bold;");
    ui->label5->setStyleSheet("color:"+QString(color)+";font : bold;");
    QFileInfoList list = image->entryInfoList();
    int i=0;
    QFileInfo fileInfo;
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
    connect(ui->okButton,SIGNAL(clicked()),parent,SLOT(ajoutIngredient()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
}

Ingredient* FenetreAjoutIngredient::creerIngredient()
{
    Ingredient* ingTemp = new Ingredient(ui->le_nom->text(), ui->cb_type->currentIndex(), ui->sb_quantite->value(),ui->comboBox->currentText(),ui->de_date->date(),ui->cb_image->currentData().toString());
    GestionDeFichiers::ajoutFichier(NULL,ingTemp);
    return ingTemp;
}


FenetreAjoutIngredient::~FenetreAjoutIngredient()
{
    delete ui;
}

