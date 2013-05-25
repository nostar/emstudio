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
QList<unsigned short> ParameterWidget::getLocationIdList()
{
	QList<unsigned short> retval;
	for (int i=0;i<m_fieldConfigList.size();i++)
	{
		if (!retval.contains(m_fieldConfigList[i].second.locationId()))
		{
			retval.append(m_fieldConfigList[i].second.locationId());
		}
	}
	return retval;
}
void ParameterWidget::updateValue(unsigned short locationid,QByteArray block)
{
	for (int i=0;i<m_fieldConfigList.size();i++)
	{
		if (m_fieldConfigList[i].second.locationId() == locationid)
		{

			QString valstr = "";
			for (int k=0;k<m_fieldConfigList[i].second.size();k++)
			{
				unsigned int value = 0;
				for (int j=0;j<m_fieldConfigList[i].second.elementSize();j++)
				{
					qDebug() << (unsigned char)block[m_fieldConfigList[i].second.offset() + (k * m_fieldConfigList[i].second.elementSize()) + j];
					value += ((unsigned char)block[m_fieldConfigList[i].second.offset() + (k * m_fieldConfigList[i].second.elementSize()) + j]) << (8 * (m_fieldConfigList[i].second.elementSize() - (j+1)));
				}
				//userValue = (ecuValue + translate) * scale
				valstr += QString::number(calcAxis(value,m_fieldConfigList[i].second.calc())) + ",";
			}
			valstr = valstr.mid(0,valstr.length()-1);
			m_nameToLineEditMap[m_fieldConfigList[i].first.variable]->setText(valstr);

		}
	}
}

void ParameterWidget::addParam(QString title,DialogField field,ConfigBlock block)
{
	qDebug() << "Title:" << title;
	qDebug() << "Field:" << field.title << field.variable;
	qDebug() << "Found config block:" << block.type();
	qDebug() << block.name();
	m_fieldConfigList.append(QPair<DialogField,ConfigBlock>(field,block));
	//This is the config block.
	if (block.type() == "scalar")
	{
		QHBoxLayout *layout = new QHBoxLayout();
		QLabel *label = new QLabel(scrollArea);
		label->show();
		label->setText(field.title);
		layout->addWidget(label);
		QLineEdit *edit = new QLineEdit(scrollArea);
		m_nameToLineEditMap[field.variable] = edit;
		edit->show();
		layout->addWidget(edit);
		scrollArea->layout()->addItem(layout);
		//lineEditToConfigBlockMap[edit] = m_memoryConfigBlockList[j];
	}
	//fieldlist[i].condition
}
double ParameterWidget::calcAxis(unsigned short val,QList<QPair<QString,double> > metadata)
{
	if (metadata.size() == 0)
	{
		return val;
	}
	double newval = val;
	for (int j=0;j<metadata.size();j++)
	{
		if (metadata[j].first == "add")
		{
			newval += metadata[j].second;
		}
		else if (metadata[j].first == "sub")
		{
			newval -= metadata[j].second;
		}
		else if (metadata[j].first == "mult")
		{
			newval *= metadata[j].second;
		}
		else if (metadata[j].first == "div")
		{
			newval /= metadata[j].second;
		}
	}
	return newval;
}
