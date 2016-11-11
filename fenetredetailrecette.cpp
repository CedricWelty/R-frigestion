#include "fenetredetailrecette.h"
#include "ui_fenetredetailrecette.h"

static const char color[] = "#FF4500";

fenetreDetailRecette::fenetreDetailRecette(Recette *recette, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fenetreDetailRecette)
{
    ui->setupUi(this);
    this->recette = recette;
    this->setModal(true);
    this->setWindowFlags(Qt::Popup);
    ui->labelNom->setStyleSheet("color:" + QString(color) + ";font : bold;text-decoration:underline;font-size:18px;");
    ui->label1->setStyleSheet("color:" + QString(color) + ";font : bold;");
    ui->label2->setStyleSheet("color:"+ QString(color)+";font : bold;");
    ui->label3->setStyleSheet("color:"+QString(color)+";font : bold;");
    initLabels();
}

void fenetreDetailRecette::initLabels()
{
    QString ingredients;
    QList<Ingredient*> liste = recette->getListIngredients();
    foreach(Ingredient* ing, liste)
    {
            if(ingredients == NULL)
                ingredients += "- " + ing->toStringDetail();
            else
                ingredients +=  "\n- " + ing->toStringDetail() ;
    }
    ui->labelNom->setAlignment(Qt::AlignCenter);
    ui->labelNom->setText(recette->getNom());
    ui->labelDuree->setText(recette->getDureePreparation());
    ui->labelImage->setStyleSheet("QLabel#labelImage{ border-image: url("+recette->getCheminImage()+") 0 0 0 0 stretch stretch; }");
    ui->labelIngredients->setText(ingredients);
    ui->labelEtapes->setText(recette->getEtapesPreparation());
}

fenetreDetailRecette::~fenetreDetailRecette()
{
    delete ui;
}
