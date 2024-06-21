#include <QKeyEvent>
#include <QLineEdit>
#include <QFocusEvent>
#include <QWheelEvent>
#include "SpinRemoveUpDown.h"

struct SpinRemoveUpDown::privateStruct
{
    bool mIsWheelDisalbled = false;
};

SpinRemoveUpDown::SpinRemoveUpDown(QWidget *parent) :
                                                      QSpinBox(parent),
                                                      d(new privateStruct)
{
    this->setKeyboardTracking(false);
    this->setAccelerated(true);
}

SpinRemoveUpDown::~SpinRemoveUpDown()
{

}

void SpinRemoveUpDown::setWheelEventDisable(bool isDisable)
{
    d->mIsWheelDisalbled = isDisable;
}

void SpinRemoveUpDown::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key::Key_Up :
        emit onPressedArrowKeyBoard(event->key());
        break;

    case Qt::Key::Key_Down:
        emit onPressedArrowKeyBoard(event->key());
        break;

    case Qt::Key::Key_Left:
        emit onPressedArrowKeyBoard(event->key());
        break;

    case Qt::Key::Key_Right:
        emit onPressedArrowKeyBoard(event->key());
        break;

    case Qt::Key::Key_Enter:
        emit onPressedArrowKeyBoard(event->key());
        break;

    case Qt::Key::Key_Delete:
        this->onDeleteActivated();
        break;

    default:
            QSpinBox::keyPressEvent(event);
        break;
    }
}

void SpinRemoveUpDown::focusInEvent(QFocusEvent* evt)
{
    QSpinBox::focusInEvent(evt);
    this->selectAll();

    emit onFocusInEventSpinBox();
}

void SpinRemoveUpDown::wheelEvent(QWheelEvent *event)
{
    if(d->mIsWheelDisalbled == true)
        event->ignore();
}

void SpinRemoveUpDown::onDeleteActivated()
{
    this->setValue(0);
}
