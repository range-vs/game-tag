#pragma once

#include <QtWidgets>

class CustomLabel : public QLabel
{
Q_OBJECT

public:
    CustomLabel(QWidget* obj = nullptr): QLabel(obj){}
    CustomLabel(QString ttl, QWidget* obj = nullptr): QLabel(ttl, obj){}

    CustomLabel* clone()
    {
        auto* elem(new CustomLabel(text()));
        elem->setCursor(cursor());
        elem->setLineWidth(lineWidth());
        elem->setAlignment(alignment());
        elem->setFont(font());
        elem->setPalette(palette());
        elem->setAutoFillBackground(autoFillBackground());
        elem->setEnabled(isEnabled());
        return elem;
    }

    bool hasHeightForWidth()const override
    {
        return true;
    }

    int heightForWidth( int w )const override
    {
        return w;
    }


signals:
    void unpressed();
    void pressed();

protected:
    bool event(QEvent* evt) override
    {
        switch(evt->type())
        {
            case(QEvent :: MouseButtonRelease):
            {
                emit unpressed();
                break;
            }
            case(QEvent :: MouseButtonPress):
            {
                emit pressed();
                break;
            }
        }
        return QWidget::event(evt);
    }
};
