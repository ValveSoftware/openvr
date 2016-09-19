#include "overlaywidget.h"
#include "ui_overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OverlayWidget)
{
    ui->setupUi(this);
}

OverlayWidget::~OverlayWidget()
{
    delete ui;
}

void OverlayWidget::on_pushButton_clicked()
{
    QApplication::quit();
}
