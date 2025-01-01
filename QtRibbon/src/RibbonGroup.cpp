
#include "RibbonGroup.h"


RibbonGroup::RibbonGroup(const QString& title, QWidget* parent)
  : QWidget(parent)
{
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(1);
    gridLayout->setContentsMargins(1, 1, 1, 1);
    vLayout = new QVBoxLayout();
    vLayout->setSpacing(1);
    vLayout->setContentsMargins(2, 2, 2, 2);
    hLayout = new QHBoxLayout();
    hLayout->setSpacing(1);

    vLayout->addLayout(hLayout);

    hLine = new QFrame(this);
    hLine->setFrameShadow(QFrame::Raised);
    hLine->setFrameShape(QFrame::HLine);
    // 设置透明度为50%
    //hLine->setStyleSheet("background-color: rgba(250, 250, 250, 0.5);");

    vLayout->addWidget(hLine);

    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setText(title);

    vLayout->addWidget(label);

    gridLayout->addLayout(vLayout, 0, 0, 1, 1);

    vLine = new QFrame(this);
    vLine->setFrameShadow(QFrame::Raised);
    vLine->setFrameShape(QFrame::VLine);

    gridLayout->addWidget(vLine, 0, 1, 1, 1);
}

void RibbonGroup::setTitle(const QString &title)
{
    label->setText(title);
}

QString RibbonGroup::title()
{
    return label->text();
}

void RibbonGroup::addButton(QToolButton* button)
{
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    button->setMinimumSize(48, 48);
    button->setAutoRaise(true);
    button->setIconSize(QSize(32, 32));
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    hLayout->addWidget(button);
}

void RibbonGroup::addWidget(QWidget* widget)
{
    widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    widget->setMinimumSize(48, 48);

    hLayout->addWidget(widget);
}
