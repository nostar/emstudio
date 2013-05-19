#ifndef PARAMETERVIEW_H
#define PARAMETERVIEW_H

#include <QWidget>
#include "headers.h"
#include "ui_parameterview.h"
#include "configblock.h"
#include "emsdata.h"

class ParameterView : public QWidget
{
	Q_OBJECT
public:
	explicit ParameterView(QWidget *parent = 0);
	void passMenuList(MenuSetup menu);
	void passConfigBlockList(QMap<QString,QList<ConfigBlock> > blocklist);
	void generateDialog(QString title,QList<DialogField> fieldlist);
	void passEmsData(EmsData *data);
	void updateValues();
	double calcAxis(unsigned short val,QList<QPair<QString,double> > metadata);
private:
	QMap<QLineEdit*,ConfigBlock> lineEditToConfigBlockMap;
	QMap<QWidget*,QList<DialogField> widgetToFieldMap;
	EmsData *m_emsData;
	MenuSetup m_metaMenu;
	QList<ConfigBlock> m_memoryConfigBlockList;
	Ui::Form ui;
signals:

public slots:
	void currentItemChanged(QTreeWidgetItem *current,QTreeWidgetItem *prev);
	void itemSelectionChanged();
};

#endif // PARAMETERVIEW_H
