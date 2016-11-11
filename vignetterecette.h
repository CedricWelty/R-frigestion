#ifndef VIGNETTERECETTE_H
#define VIGNETTERECETTE_H

#include <QWidget>
#include <recette.h>
#include <QPainter>
#include <fenetredetailrecette.h>
#include <QGraphicsDropShadowEffect>
class MainWindow;

namespace Ui {
class VignetteRecette;
}

class VignetteRecette : public QWidget
{
    Q_OBJECT

public:
    explicit VignetteRecette(int width,Recette* recette, MainWindow *parent);
    ~VignetteRecette();
    Recette *getRecette();
    void actualiserAffichage();

private:
    Recette* recette;
    Ui::VignetteRecette *ui;
    void initLabels();
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);
    MainWindow* window;


protected:
    void mousePressEvent ( QMouseEvent * event );


private slots:
    void supprimerVignette();
    void ouvrirModif();

};

#endif // VIGNETTERECETTE_H
