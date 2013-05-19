#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "configblock.h"
#include "headers.h"
class ParameterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ParameterWidget(QWidget *parent = 0);
	void addParam(QString title,DialogField field,ConfigBlock block);
private:
	QScrollArea *scrollArea;
	QVBoxLayout *mainLayout;
	QWidget *scrollWidget;
signals:
	
public slots:
	
};

#endif // PARAMETERWIDGET_H
