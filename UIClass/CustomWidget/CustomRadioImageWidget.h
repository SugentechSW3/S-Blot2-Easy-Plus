#ifndef CUSTOMRADIOIMAGEWIDGET_H
#define CUSTOMRADIOIMAGEWIDGET_H

#include <QRadioButton>

class CustomRadioImageWidget : public QRadioButton
{
    Q_OBJECT
public:
    explicit CustomRadioImageWidget(QWidget *parent = nullptr);
    ~CustomRadioImageWidget();

private:
    void initStyleSheet();
};

#endif // CUSTOMRADIOIMAGEWIDGET_H
