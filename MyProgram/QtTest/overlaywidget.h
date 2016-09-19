#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>

namespace Ui {
class OverlayWidget;
}

class OverlayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OverlayWidget(QWidget *parent = 0);
    ~OverlayWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::OverlayWidget *ui;
};

#endif // OVERLAYWIDGET_H
