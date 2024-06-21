#include "CirclePrograssDialog.h"
#include "ui_CirclePrograssDialog.h"

CirclePrograssDialog::CirclePrograssDialog(QWidget *parent) :
    CDialogParentWidget(parent),
    ui(new Ui_CirclePrograssDialog)
{
    ui->setupUi(this);
    this->hide();
}

CirclePrograssDialog::~CirclePrograssDialog()
{
}


void CirclePrograssDialog::start()
{
    this->show();
    this->raise();
    ui->widget->startInfiniteAnimation();
}


void CirclePrograssDialog::stop()
{
    this->hide();
    ui->widget->stopInfiniteAnimation();
}
