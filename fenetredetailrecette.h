#ifndef FENETREDETAILRECETTE_H
#define FENETREDETAILRECETTE_H

#include <QDialog>
#include <recette.h>

namespace Ui {
class fenetreDetailRecette;
}

class fenetreDetailRecette : public QDialog
{
    Q_OBJECT

public:
    explicit fenetreDetailRecette(Recette* recette, QWidget *parent = 0);
    ~fenetreDetailRecette();

private:
    Ui::fenetreDetailRecette *ui;
    Recette* recette;

    void initLabels();

};

#endif // FENETREDETAILRECETTE_H
