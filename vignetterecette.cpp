#include "vignetterecette.h"
#include "ui_vignetterecette.h"
#include <mainwindow.h>
VignetteRecette::VignetteRecette(int width, Recette* recette, MainWindow *parent) :
    QWidget(parent),
    ui(new Ui::VignetteRecette)
{
    ui->setupUi(this);
    window = parent;
    this->recette = recette;
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    this->setMaximumWidth(width);
    this->setMaximumHeight(width);
    ui->labelImage->setMinimumHeight(width * 0.5);
    this->setStyleSheet("QWidget#VignetteRecette{border-bottom: 3px solid #BDBDBD;border-right : 1px solid #BDBDBD;background-color : white;}"
                        "QLabel{font:16px;color:black;} ");
    initLabels();
    connect(ui->buttonSuppr,SIGNAL(clicked(bool)),this,SLOT(supprimerVignette()));
    connect(ui->modifButton, SIGNAL(clicked(bool)),this,SLOT(ouvrirModif()));

    this->setAttribute(Qt::WA_Hover, true);
    this->show();
}

void VignetteRecette::ouvrirModif()
{
    window->ouvrirFenArModif(this->recette);
}

void VignetteRecette::actualiserAffichage()
{
    recette->actualiserRealisable();
    initLabels();
}

void VignetteRecette::initLabels()
{
    ui->labelNom->setText(recette->getNom());
    ui->labelDureePrep->setText((recette->getDureePreparation()));
    recette->actualiserRealisable();
    if(recette->isRealisable())
        ui->labelNom->setStyleSheet("QLabel#labelNom{font-weight: bold;color:green;}");
    else
        ui->labelNom->setStyleSheet("QLabel#labelNom{font-weight: bold;color:red;}");
    ui->labelImage->setStyleSheet("QLabel#labelImage{ border-image: url("+recette->getCheminImage()+") 0 0 0 0 stretch stretch; }" );
    repaint();
}

//Utile pour que l'affichage se fasse correctement
void VignetteRecette::paintEvent(QPaintEvent *) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

void VignetteRecette::mousePressEvent ( QMouseEvent *)
{
    fenetreDetailRecette* fenDR = new fenetreDetailRecette(recette);
    fenDR->show();
}

void VignetteRecette::supprimerVignette()
{
    window->supprimerVignetteRecette(this);
}

Recette *VignetteRecette::getRecette()
{
    return recette;
}


void VignetteRecette::enterEvent(QEvent * event)
{
    QGraphicsDropShadowEffect *g = new QGraphicsDropShadowEffect(this);
    g->setBlurRadius(7);
    g->setOffset(0,0);
    this->setGraphicsEffect(g);
    QWidget::enterEvent(event);
}

void VignetteRecette::leaveEvent(QEvent * event)
{
    this->setGraphicsEffect(NULL);
    QWidget::leaveEvent(event);
}


VignetteRecette::~VignetteRecette()
{
    delete ui;
}
