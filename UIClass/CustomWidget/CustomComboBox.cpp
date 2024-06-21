#include "CustomComboBox.h"
#include "Analysis/GlobalDataStruct.h"
#include "Analysis/ConfigInformation.h"

CustomComboBox::CustomComboBox(QWidget *parent) :
                                                  QComboBox(parent)
{
    this->initStyleSheet();
    connect(this, &QComboBox::currentTextChanged, this, &CustomComboBox::onTextColorChanged);
}

CustomComboBox::~CustomComboBox()
{

}

void CustomComboBox::initStyleSheet()
{
    QString arrowStyle;
    arrowStyle.append("QComboBox::down-arrow {");
    arrowStyle.append("image: url(:/Image/Image/Information/arrow_down.png);");
    arrowStyle.append("width: 14px;");
    arrowStyle.append("height: 14px;}");

    this->setStyleSheet(arrowStyle);
}

void CustomComboBox::addItemForPanelColorText(const QString& itemName)
{
    auto idx = this->count();
    auto textColor = this->findColor(itemName);

    this->addItem(itemName);
    this->setItemData(idx, textColor, Qt::TextColorRole);
}

void CustomComboBox::addItemForPanelColorText(const QString& item, const QVariant& data)
{
    auto idx = this->count();
    auto textColor = this->findColor(item);

    this->addItem(item, data);
    this->setItemData(idx, textColor, Qt::TextColorRole);
}

void CustomComboBox::addItemForPanelColorText(const QStringList& itemList)
{
    for(auto& itr : itemList)
        this->addItemForPanelColorText(itr);
}

void CustomComboBox::onTextColorChanged()
{
    auto text = this->currentData(Qt::TextColorRole).toString();
    auto p = this->palette();

    QBrush brush;
    brush.setColor(text);
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(QPalette::Active, QPalette::Text, brush);

    this->setPalette(p);
}

QColor CustomComboBox::findColor(const QString &itemName)
{
    QColor color(Qt::black);
    auto config = ConfigInformation::getInstance();
    auto stripColor  = config->getAllergyColor(itemName);

    switch((int)stripColor)
    {
    case ConfigInformation::STRIP_COLOR_GRAY:
        color = QColor("#6FA0D9");
        break;

    case ConfigInformation::STRIP_COLOR_GREEN:
        color = QColor("#00DB9B");
        break;

    case ConfigInformation::STRIP_COLOR_PINK:
        color = QColor("#FF45C4");
        break;
    }

    return color;
}
