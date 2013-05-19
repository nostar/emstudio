#include "parameterwidget.h"
#include <QScrollArea>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
ParameterWidget::ParameterWidget(QWidget *parent) : QWidget(parent)
{
	scrollArea = new QScrollArea(this);
	//scrollWidget = new QWidget();
	//scrollWidget->setLayout(new QVBoxLayout());
	//scrollArea->setWidget(scrollWidget);
	scrollArea->setLayout(new QVBoxLayout());
	mainLayout = new QVBoxLayout();
	this->setLayout(mainLayout);
	mainLayout->addWidget(scrollArea);
	scrollArea->show();
	//scrollWidget->show();
	//scrollArea->show();
}

void ParameterWidget::addParam(QString title,DialogField field,ConfigBlock block)
{
	qDebug() << "Title:" << title;
	qDebug() << "Field:" << field.title << field.variable;
	qDebug() << "Found config block:" << block.type();
	qDebug() << block.name();
	//This is the config block.
	if (block.type() == "scalar")
	{
		QHBoxLayout *layout = new QHBoxLayout();
		QLabel *label = new QLabel(scrollArea);
		label->show();
		label->setText(field.title);
		layout->addWidget(label);
		QLineEdit *edit = new QLineEdit(scrollArea);
		edit->show();
		layout->addWidget(edit);
		scrollArea->layout()->addItem(layout);
		//lineEditToConfigBlockMap[edit] = m_memoryConfigBlockList[j];
	}
	//fieldlist[i].condition
}
