#include "parameterview.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
ParameterView::ParameterView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.parameterTreeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
	connect(ui.parameterTreeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(itemSelectionChanged()));
}
void ParameterView::itemSelectionChanged()
{
	return;
	/*qDebug() << "Item selection changed";
	if (ui.parameterTreeWidget->selectedItems().size() <= 0)
	{
		return;
	}
	QTreeWidgetItem *current = ui.parameterTreeWidget->selectedItems()[0];
	qDebug() << "Menu selected:" << current->text(0);
	if (m_metaMenu.dialogmap.contains(current->text(0)))
	{
		qDebug() << "Dialog:" << m_metaMenu.dialogmap[current->text(0)];
	}*/
}

void ParameterView::currentItemChanged(QTreeWidgetItem *current,QTreeWidgetItem *prev)
{
	Q_UNUSED(prev);
	if (!current)
	{
		return;
	}
	/*if (m_metaMenu.menunamemap.contains(current->text(0)))
	{
		if (m_metaMenu.dialogmap.contains(current->text(0)))
		{
			qDebug() << "Dialog:" << m_metaMenu.dialogmap[current->text(0)];
		}
	}*/
	if (current->parent() == 0)
	{
		//It is a top level item. don't do anything
		return;
	}
	for (int i=0;i<m_metaMenu.menulist.size();i++)
	{
		if (m_metaMenu.menulist[i].title == current->parent()->text(0))
		{
			//This is our menu!
			for (int j=0;j<m_metaMenu.menulist[i].subMenuList.size();j++)
			{
				if (m_metaMenu.menulist[i].subMenuList[j].title == current->text(0))
				{
					//This is our current item!
					if (!m_metaMenu.menulist[i].subMenuList[j].is_seperator)
					{
						qDebug() << "Variable:" << m_metaMenu.menulist[i].subMenuList[j].variable;
						for (int k=0;k<m_metaMenu.dialoglist.size();k++)
						{
							if (m_metaMenu.dialoglist[k].variable == m_metaMenu.menulist[i].subMenuList[j].variable)
							{
								qDebug() << "Found Dialog:" << m_metaMenu.dialoglist[k].title << m_metaMenu.dialoglist[k].fieldList.size();
								generateDialog(m_metaMenu.dialoglist[k].title,m_metaMenu.dialoglist[k].fieldList);
								//Generate a dialog here.
							}

						}
					}
				}
			}
		}
	}

}
void ParameterView::generateDialog(QString title,QList<DialogField> fieldlist)
{
	QWidget *widget = new QWidget();
	widget->setGeometry(0,0,800,600);
	widget->setWindowTitle(title);
	Q_UNUSED(fieldlist);
	widget->show();
	widget->setLayout(new QVBoxLayout());
	for (int i=0;i<fieldlist.size();i++)
	{
		qDebug() << "Field:" << fieldlist[i].title << fieldlist[i].variable;
		for (int j=0;j<m_memoryConfigBlockList.size();j++)
		{
			//qDebug() << "Config block:" << m_memoryConfigBlockList[j].type();
			if (m_memoryConfigBlockList[j].name() == fieldlist[i].variable)
			{
				qDebug() << "Found config block:" << m_memoryConfigBlockList[j].type();
				qDebug() << m_memoryConfigBlockList[j].name() << j;
				//This is the config block.
				if (m_memoryConfigBlockList[j].type() == "scalar")
				{
					QHBoxLayout *layout = new QHBoxLayout();
					QLabel *label = new QLabel(widget);
					label->show();
					label->setText(fieldlist[i].title);
					layout->addWidget(label);
					QTextEdit *edit = new QTextEdit(widget);
					edit->show();
					layout->addWidget(edit);
					widget->layout()->addItem(layout);
				}
			}
		}
		//fieldlist[i].condition
	}
}

void ParameterView::passMenuList(MenuSetup menu)
{
	m_metaMenu = menu;
	for (int i=0;i<menu.menulist.size();i++)
	{
		//menu.menulist.
		ui.parameterTreeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList() << menu.menulist[i].title));
		QTreeWidgetItem *item = ui.parameterTreeWidget->topLevelItem(ui.parameterTreeWidget->topLevelItemCount()-1);
		for (int j=0;j<menu.menulist[i].subMenuList.size();j++)
		{
			//if (menu.menunamemap.contains(menu.menulist[i].second[j]))
			//{
			//	item->addChild(new QTreeWidgetItem(QStringList() << menu.menunamemap[menu.menulist[i].second[j]]));
			//}
			//else
			//{
			//	item->addChild(new QTreeWidgetItem(QStringList() << menu.menulist[i].second[j]));
			//}
			item->addChild(new QTreeWidgetItem(QStringList() << menu.menulist[i].subMenuList[j].title));
		}
	}
}

void ParameterView::passConfigBlockList(QMap<QString,QList<ConfigBlock> > blocklist)
{
	for (QMap<QString,QList<ConfigBlock> >::const_iterator i=blocklist.constBegin();i!=blocklist.constEnd();i++)
	{
		//ui.parameterTreeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList() << i.key()));
		//QTreeWidgetItem *item = ui.parameterTreeWidget->topLevelItem(ui.parameterTreeWidget->topLevelItemCount()-1);
		for (int j=0;j<i.value().size();j++)
		{
		//	item->addChild(new QTreeWidgetItem(QStringList() << i.value().at(j).name()));
			m_memoryConfigBlockList.append(i.value().at(j));
		}
	}
}

