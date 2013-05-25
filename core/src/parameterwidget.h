#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLineEdit>
#include "configblock.h"
#include "headers.h"
class ParameterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ParameterWidget(QWidget *parent = 0);
	void addParam(QString title,DialogField field,ConfigBlock block);
	QList<unsigned short> getLocationIdList();
	void updateValue(unsigned short locationid,QByteArray block);
private:
	double calcAxis(unsigned short val,QList<QPair<QString,double> > metadata);
	QList<QPair<DialogField,ConfigBlock> > m_fieldConfigList;
	QMap<QString,QLineEdit*> m_nameToLineEditMap;
	QScrollArea *scrollArea;
	QVBoxLayout *mainLayout;
	QWidget *scrollWidget;
signals:
	
public slots:
	
};

#endif // PARAMETERWIDGET_H
