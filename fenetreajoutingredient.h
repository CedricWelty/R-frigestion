#ifndef FENETREAJOUTINGREDIENT_H
#define FENETREAJOUTINGREDIENT_H

#include <QDialog>
#include <ingredient.h>

namespace Ui {
class FenetreAjoutIngredient;
}

class FenetreAjoutIngredient : public QDialog
{
    Q_OBJECT

public:
    explicit FenetreAjoutIngredient(QWidget *parent = 0);
    ~FenetreAjoutIngredient();
    Ingredient* creerIngredient();

private:
    Ui::FenetreAjoutIngredient *ui;
    QList<QString> creerListeIngredients();

#endif // FENETREAJOUTINGREDIENT_H

};

