#include "vignetteingredient.h"
#include "ui_vignetteingredient.h"
#include <mainwindow.h>

VignetteIngredient::VignetteIngredient(int width, Ingredient* ingredient, MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::VignetteIngredient)
{
    ui->setupUi(this);
    window = parent;
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(retraitQuantite()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(ajoutQuantite()));
    connect(ui->sp_quantite, SIGNAL(editingFinished()), this, SLOT(verifierQuantite()));
    connect(ui->pb_supprimer, SIGNAL(clicked(bool)), this, SLOT(supprimerVignette()));
    this->ingredient = ingredient;
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    this->setMaximumWidth(width);
    this->setMaximumHeight(width);
    ui->labelImage->setMinimumHeight(width * 0.5);
    ui->labelImage->setMaximumSize(width * 0.9, width * 0.56);

   /* QPolygon forme;
    forme << QPoint(0, 20) << QPoint(40, 0) << QPoint(40, 40);
    ui->pushButton->setMask(QRegion(forme));
    forme.clear();
    forme << QPoint(0, 0) << QPoint(40, 20) << QPoint(0, 40);
    ui->pushButton_2->setMask(QRegion(forme));*/

    ui->label_type->setPixmap(ingredient->getTypeColor());
    this->setStyleSheet("QWidget#VignetteIngredient{border-bottom: 3px solid #BDBDBD;border-right : 1px solid #BDBDBD;background-color : white;}"
                        "QLabel{font:16px;color:black;} ");
    initLabels();
    verifierPeremption();

    this->setAttribute(Qt::WA_Hover, true); //On définit la vérification du Hover
    this->show();
}

void VignetteIngredient::enterEvent(QEvent * event)
{
    QGraphicsDropShadowEffect *g = new QGraphicsDropShadowEffect(this);
    g->setBlurRadius(7);
    g->setOffset(0,0);
    this->setGraphicsEffect(g); //On ajoute l'ombre au Widget
    ui->label_type->setPixmap(ingredient->getTypeColorLettre()); //On modifie l'icone pour afficher la lettre
    QWidget::enterEvent(event);
}

void VignetteIngredient::leaveEvent(QEvent * event)
{
    this->setGraphicsEffect(NULL); //On enleve l'ombre
    ui->label_type->setPixmap(ingredient->getTypeColor()); //On modifie l'icone
    QWidget::leaveEvent(event);
}

void VignetteIngredient::ajoutQuantite()
{
    ui->sp_quantite->setValue(ingredient->getQuantite() + ingredient->getIncrement());
    verifierQuantite();
}

void VignetteIngredient::retraitQuantite()
{
    ui->sp_quantite->setValue(ingredient->getQuantite() - ingredient->getIncrement());
    verifierQuantite();
}

void VignetteIngredient::initLabels()
{
    if(ingredient->getQuantite()==0) ingredient->setQuantite(1);
    ui->sp_quantite->setValue(ingredient->getQuantite());
    ui->label_unite->setText(ingredient->getUnite());
    ui->label_nom->setText(ingredient->getNom());
    ui->labelImage->setStyleSheet("QLabel#labelImage{ border-image: url("+ingredient->getCheminImage()+") 0 0 0 0 stretch stretch; }");
}

//Utile pour que l'affichage se fasse correctement
void VignetteIngredient::paintEvent(QPaintEvent*)
{
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void VignetteIngredient::verifierPeremption()
{
    int diff = QDate::currentDate().daysTo(ingredient->getDate()); //Différence entre la date actuelle et la date de péremption
    if(diff <= 0) //Ingrédient périmé
    {
        this->setStyleSheet("QWidget#VignetteIngredient{border-bottom: 3px solid #BDBDBD;border-right : 1px solid #BDBDBD;background-color : white;}"
                            "QLabel{font:16px;color: black;}"
                            "QLabel#labelRestant{font-weight: bold;color:red;}");
        if(diff == 0)
            ui->labelRestant->setText("Périmé aujourd'hui");
        else if(diff == -1)
            ui->labelRestant->setText("Périmé depuis " + QString::number(-diff) + " jour");
        else
            ui->labelRestant->setText("Périmé depuis " + QString::number(-diff) + " jours");
    }
    else if(diff <= 2) // Ingrédient à manger rapidement
    {
       this->setStyleSheet("QWidget#VignetteIngredient{border-bottom: 3px solid #BDBDBD;border-right : 1px solid #BDBDBD;background-color : white;}"
                            "QLabel{font:16px;color: black;}"
                            "QLabel#labelRestant{font-weight: bold;color:orange;}");
        if(diff == 1)
            ui->labelRestant->setText("Périme demain");
        else
            ui->labelRestant->setText(QString::number(diff) + " jours restants");
    }
    else // Cas normal
    {
        this->setStyleSheet("QWidget#VignetteIngredient{border-bottom: 3px solid #BDBDBD;border-right : 1px solid #BDBDBD;background-color : white;}"
                            "QLabel{font:16px;color: black;}");
        ui->labelRestant->setText(QString::number(diff) + " jours restants");
    }

}

void VignetteIngredient::verifierQuantite()
{
    if(ui->sp_quantite->value() <= 0)
    {
        if(!window->supprimerVignetteIngredient(this))
        {
            ui->sp_quantite->setValue(ingredient->getQuantite());
        }
    }
    ingredient->setQuantite(ui->sp_quantite->value());
    window->reecrireFichier();
    window->actualiserVignettesRecettes();
}

void VignetteIngredient::supprimerVignette()
{
    window->supprimerVignetteIngredient(this);
}

Ingredient *VignetteIngredient::getIngredient()
{
    return ingredient;
}

VignetteIngredient::~VignetteIngredient()
{
    delete ui;
}
