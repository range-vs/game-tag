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
        CustomLabel* elem = new CustomLabel(text());
        elem->setCursor(cursor());
        elem->setLineWidth(lineWidth());
        elem->setFixedSize(size());
        elem->setAlignment(alignment());
        elem->setFont(font());
        elem->setPalette(palette());
        elem->setAutoFillBackground(autoFillBackground());
        elem->setEnabled(isEnabled());
        return elem;
    }

signals:
    void clicked();

protected:
    bool event(QEvent* evt) override
    {
        switch(evt->type())
        {
            case(QEvent :: MouseButtonRelease):
            {
                emit clicked();
                break;
            }
        }
        return QWidget::event(evt);
    }
};
