

#include "RibbonTab.h"
#include "RibbonGroup.h"


RibbonTab::RibbonTab(QWidget* parent)
    : QWidget(parent)
{
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setFrameShadow(QFrame::Plain);
    scrollArea->setLineWidth(0);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);

    scrollAreaContent = new QWidget();
    scrollAreaContent->setGeometry(QRect(0, 0, 400, 90));

    scrollAreaGridLayout = new QGridLayout(scrollAreaContent);
    scrollAreaGridLayout->setSpacing(0);
    scrollAreaGridLayout->setContentsMargins(0, 0, 0, 0);

    spacer = new QWidget(scrollAreaContent);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(spacer->sizePolicy().hasHeightForWidth());
    spacer->setSizePolicy(sizePolicy);

    scrollAreaGridLayout->addWidget(spacer, 0, 1, 1, 1);

    hLayout = new QHBoxLayout();
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0, 3, 0, 0);

    scrollAreaGridLayout->addLayout(hLayout, 0, 0, 1, 1);

    scrollArea->setWidget(scrollAreaContent);

    gridLayout->addWidget(scrollArea, 0, 0, 1, 1);
}

RibbonGroup* RibbonTab::addGroup(const QString& groupName)
{
    RibbonGroup* group = new RibbonGroup;
    group->setTitle(groupName);

    hLayout->addWidget(group);

	return group;
};

void RibbonTab::addGroup(RibbonGroup* ribbonGroup)
{
    hLayout->addWidget(ribbonGroup);
}
void RibbonTab::addButton(const QString& groupName, QToolButton* button)
{
    RibbonGroup* ribbonButtonGroup = this->group(groupName);
    ribbonButtonGroup ?
        ribbonButtonGroup->addButton(button) :
        addGroup(groupName)->addButton(button);
}
void RibbonTab::addWidget(const QString& groupName, QWidget* widget)
{
	RibbonGroup* ribbonButtonGroup = this->group(groupName);
	ribbonButtonGroup ? 
        ribbonButtonGroup->addWidget(widget) : 
        addGroup(groupName)->addWidget(widget);  
};

RibbonGroup* RibbonTab::group(const QString& groupName)
{
	// Find ribbon group
	RibbonGroup* ribbonGroup = nullptr;
	for (int i = 0; i < hLayout->count(); i++)
	{
		RibbonGroup* group = static_cast<RibbonGroup*>(hLayout->itemAt(i)->widget());
		if (group->title().toLower() == groupName.toLower())
		{
			ribbonGroup = group;
			break;
		}
	}
	return ribbonGroup;
}

RibbonGroup* RibbonTab::operator[](const QString& groupName)
{
	return group(groupName);
}
