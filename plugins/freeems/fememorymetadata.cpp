/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2013  Michael Carpenter (malcom2073@gmail.com)                     *
 *                                                                                  *
 * This file is a part of EMStudio                                                  *
 *                                                                                  *
 * EMStudio is free software; you can redistribute it and/or                        *
 * modify it under the terms of the GNU Lesser General Public                       *
 * License as published by the Free Software Foundation, version                    *
 * 2.1 of the License.                                                              *
 *                                                                                  *
 * EMStudio is distributed in the hope that it will be useful,                      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 * Lesser General Public License for more details.                                  *
 *                                                                                  *
 * You should have received a copy of the GNU Lesser General Public                 *
 * License along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   *
 ************************************************************************************/

#include "fememorymetadata.h"
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <qjson/parser.h>
//#include <QMessageBox>
#include <QVariant>
#include "configblock.h"
FEMemoryMetaData::FEMemoryMetaData()
{
}
bool FEMemoryMetaData::parseMetaData(QString json)
{
	QJson::Parser parser;
	QVariant top = parser.parse(json.toStdString().c_str());
	if (!top.isValid())
	{
		QString errormsg = QString("Error parsing JSON from config file on line number: ") + QString::number(parser.errorLine()) + " error text: " + parser.errorString();
		//QMessageBox::information(0,"Error",errormsg);
		qDebug() << "Error parsing JSON";
		qDebug() << "Line number:" << parser.errorLine() << "error text:" << parser.errorString();
		qDebug() << "Start Json";
		qDebug() << "Json:" << json;
		qDebug() << "End Json";
		return false;
	}
	QVariantMap topmap = top.toMap();
	QVariantMap errormap = topmap["errormap"].toMap();
	QVariantMap::iterator i = errormap.begin();
	while (i != errormap.end())
	{
		bool ok = false;
		m_errorMap[i.value().toString().mid(2).toInt(&ok,16)] = i.key();
		i++;
	}

	QVariantMap ramvars = topmap["ramvars"].toMap();
	i = ramvars.begin();
	while (i != ramvars.end())
	{
		bool ok = false;
		unsigned short locid = i.key().mid(2).toInt(&ok,16);
		m_readOnlyMetaDataMap[locid] = ReadOnlyRamBlock();
		QVariantMap locidlist = i.value().toMap();
		QString title = locidlist["title"].toString();
		m_readOnlyMetaDataMap[locid].title = title;
		QVariantList locidmap = locidlist["vars"].toList();
		int offset = 0;
		for (int j=0;j<locidmap.size();j++)
		{
			QVariantMap newlocidmap = locidmap[j].toMap();
			ReadOnlyRamData rdata;
			rdata.dataTitle = newlocidmap["name"].toString();
			rdata.dataDescription = newlocidmap["title"].toString();
			rdata.locationId = locid;
			rdata.offset = offset;
			rdata.size = newlocidmap["size"].toInt();
			offset += rdata.size;
			m_readOnlyMetaDataMap[locid].m_ramData.append(rdata);
			m_readOnlyMetaData.append(rdata);
			//m_readOnlyMetaDataMap[locid].append(rdata);

		}
		/*QVariantMap::iterator j = locidmap.begin();
		while (j != locidmap.end())
		{
			if (j.key() == "title")
			{
				QString title = j.value().toString();
				qDebug() << "Location title:" << title;
			}
			else
			{
				qDebug() << j.key();
				QVariantMap valuemap = j.value().toMap();
				if (valuemap.contains("type"))
				{
					ConfigData cdata;
					cdata.configDescription = valuemap["title"].toString();
					cdata.configTitle = j.key();
					cdata.elementSize = valuemap["size"].toInt();
					cdata.locationId = locid;
					cdata.offset = valuemap["offset"].toInt();
					cdata.type = valuemap["type"].toString();
					QVariantMap calcmap = valuemap["calc"].toMap();
					QList<QPair<QString,double> > calclist;
					QVariantMap::iterator k = calcmap.begin();
					while (k != calcmap.end())
					{
						calclist.append(QPair<QString,double>(k.key(),k.value().toDouble()));
						k++;
					}
					cdata.elementCalc = calclist;
					if (valuemap["type"] == "value")
					{

					}
					else if (valuemap["type"] == "array")
					{
						cdata.arraySize = valuemap["arraysize"].toInt();
					}
					m_configMetaData.append(cdata);
				}

			}
			j++;
		}*/
		i++;
	}
	/*"lookuptables" : {
 "0x8000" : {
  "title": "IAT Transfer Table",
  "size" : "1024",
  "editable" : "false"
 },
 "0x8001" : {
  "title" : "CHT Transfer Table",
  "size" : "1024",
  "editable" : "false"
 }
}*/

	QVariantMap newconfig = topmap["newconfig"].toMap();
	i = newconfig.begin();

	while (i != newconfig.end())
	{
		MenuItem menuitem;
		QString newconfigkey = i.key();
		QVariantMap newconfigmap = i.value().toMap();
		QString newconfigtitle = newconfigmap["title"].toString();
		QVariantList valueslist = newconfigmap["values"].toList();
		QList<ConfigBlock> blocklist;
		menuitem.title = newconfigtitle;
		for (int j=0;j<valueslist.size();j++)
		{
			SubMenuItem submenu;
			QVariantMap tmpmap = valueslist[j].toMap();
			ConfigBlock block;
			QString str = tmpmap["locationid"].toString();
			bool ok = false;
			unsigned short locid = str.toInt(&ok,16);
			block.setLocationId(locid);
			block.setName(tmpmap["name"].toString());
			block.setType(tmpmap["type"].toString());
			block.setElementSize(tmpmap["sizeofelement"].toInt());
			block.setSize(tmpmap["size"].toInt());
			block.setOffset(tmpmap["offset"].toInt());
			block.setSizeOverride(tmpmap["sizeoverride"].toString());
			block.setSizeOverrideMult(tmpmap["sizeoverridemult"].toDouble());
			QList<QPair<QString, double> > calclist;
			QVariantList calcliststr = tmpmap["calc"].toList();
			for (int k=0;k<calcliststr.size();k++)
			{
				qDebug() << "XCalc:" << calcliststr[k].toMap()["type"].toString() << calcliststr[k].toMap()["value"].toDouble();
				calclist.append(QPair<QString,double>(calcliststr[k].toMap()["type"].toString(),calcliststr[k].toMap()["value"].toDouble()));
			}
			block.setCalc(calclist);
			blocklist.append(block);
			submenu.title = block.name();
			submenu.variable = block.name();
			submenu.is_seperator = false;
			submenu.parent = newconfigtitle;
			menuitem.subMenuList.append(submenu);
		}
		m_configMetaData[newconfigtitle] = blocklist;
		i++;
		m_menuSetup.menulist.append(menuitem);
	}
	QVariantMap lookups = topmap["lookuptables"].toMap();
	i = lookups.begin();
	while (i != lookups.end())
	{
		QVariantMap lookupmap = i.value().toMap();
		QString keystr = i.key();
		bool ok = false;
		unsigned short keyint = keystr.mid(2).toInt(&ok,16);
		LookupMetaData meta;
		meta.locationid = keyint;
		meta.title = lookupmap["title"].toString();
		if (lookupmap["editable"].toString().toLower() == "true")
		{
			meta.editable = true;
		}
		else
		{
			meta.editable = false;
		}
		m_lookupMetaData[keyint] = meta;
		i++;
	}
	qDebug() << m_readOnlyMetaData.size() << "Ram entries found";
	QVariantMap tables = topmap["tables"].toMap();
	i = tables.begin();
	while (i != tables.end())
	{
		QVariantMap tabledata = i.value().toMap();
		if (tabledata["type"] == "3D")
		{
			Table3DMetaData meta;
			QString id = tabledata["locationid"].toString();
			QString xtitle = tabledata["xtitle"].toString();
			QVariantList xcalc = tabledata["xcalc"].toList();
			QString xdp = tabledata["xdp"].toString();
			unsigned int size = tabledata["size"].toInt();

			QString ytitle = tabledata["ytitle"].toString();
			QVariantList ycalc = tabledata["ycalc"].toList();
			QString ydp = tabledata["ydp"].toString();

			QString ztitle = tabledata["ztitle"].toString();
			QVariantList zcalc = tabledata["zcalc"].toList();
			QString zdp = tabledata["zdp"].toString();

			QString xhighlight = tabledata["xhighlight"].toString();
			QString yhighlight = tabledata["yhighlight"].toString();

			//QVariantMap::iterator calci = xcalc.begin();
			QList<QPair<QString,double> > xcalclist;
			QList<QPair<QString,double> > ycalclist;
			QList<QPair<QString,double> > zcalclist;
			for (int j=0;j<xcalc.size();j++)
			{
				qDebug() << "XCalc:" << xcalc[j].toMap()["type"].toString() << xcalc[j].toMap()["value"].toDouble();
				xcalclist.append(QPair<QString,double>(xcalc[j].toMap()["type"].toString(),xcalc[j].toMap()["value"].toDouble()));
			}
			for (int j=0;j<ycalc.size();j++)
			{
				ycalclist.append(QPair<QString,double>(ycalc[j].toMap()["type"].toString(),ycalc[j].toMap()["value"].toDouble()));
			}
			for (int j=0;j<zcalc.size();j++)
			{
				zcalclist.append(QPair<QString,double>(zcalc[j].toMap()["type"].toString(),zcalc[j].toMap()["value"].toDouble()));
			}

			bool ok = false;
			meta.locationId = id.mid(2).toInt(&ok,16);
			meta.tableTitle = i.key();
			meta.xAxisCalc = xcalclist;
			meta.xAxisTitle = xtitle;
			meta.xDp = xdp.toInt();
			meta.yAxisCalc = ycalclist;
			meta.yAxisTitle = ytitle;
			meta.yDp = ydp.toInt();
			meta.zAxisCalc = zcalclist;
			meta.zAxisTitle = ztitle;
			meta.zDp = zdp.toInt();
			meta.size = size;
			meta.valid = true;
			meta.xHighlight = xhighlight;
			meta.yHighlight = yhighlight;
			m_table3DMetaData.append(meta);
		}
		else if (tabledata["type"] == "2D")
		{
			Table2DMetaData meta;
			QString id = tabledata["locationid"].toString();
			QString xtitle = tabledata["xtitle"].toString();
			QVariantList xcalc = tabledata["xcalc"].toList();
			QString xdp = tabledata["xdp"].toString();
			QString ytitle = tabledata["ytitle"].toString();
			QVariantList ycalc = tabledata["ycalc"].toList();
			QString ydp = tabledata["ydp"].toString();
			unsigned int size = tabledata["size"].toInt();
			QString xhighlight = tabledata["xhighlight"].toString();

			QList<QPair<QString,double> > xcalclist;
			QList<QPair<QString,double> > ycalclist;

			for (int j=0;j<xcalc.size();j++)
			{
				qDebug() << "XCalc:" << xcalc[j].toMap()["type"].toString() << xcalc[j].toMap()["value"].toDouble();
				xcalclist.append(QPair<QString,double>(xcalc[j].toMap()["type"].toString(),xcalc[j].toMap()["value"].toDouble()));
			}
			for (int j=0;j<ycalc.size();j++)
			{
				ycalclist.append(QPair<QString,double>(ycalc[j].toMap()["type"].toString(),ycalc[j].toMap()["value"].toDouble()));
			}
			bool ok = false;
			meta.locationId = id.mid(2).toInt(&ok,16);
			meta.tableTitle = i.key();
			meta.xAxisCalc = xcalclist;
			meta.xAxisTitle = xtitle;
			meta.xDp = xdp.toInt();
			meta.yAxisCalc = ycalclist;
			meta.yAxisTitle = ytitle;
			meta.yDp = ydp.toInt();
			meta.size = size;
			meta.valid = true;
			meta.xHighlight = xhighlight;
			m_table2DMetaData.append(meta);
		}
		i++;
	}
	return true;
}
bool FEMemoryMetaData::hasConfigMetaData(QString name)
{
	return m_configMetaData.contains(name);
}

const QMap<QString,QList<ConfigBlock> > FEMemoryMetaData::configMetaData()
{
	return m_configMetaData;
}

const QList<ConfigBlock> FEMemoryMetaData::getConfigMetaData(QString name)
{
	if (m_configMetaData.contains(name))
	{
		return m_configMetaData[name];
	}
	else
	{
		return QList<ConfigBlock>();
	}
}

bool FEMemoryMetaData::loadMetaDataFromFile(QString filestr)
{
	qDebug() << "Loading config file from:" << filestr;
	QFile file(filestr);
	if (!file.open(QIODevice::ReadOnly))
	{
		//QMessageBox::information(0,"Error","Error opening config file: " + file.errorString());
		return false;
		//Can't open the file.
	}
	QByteArray filebytes = file.readAll();
	qDebug() << "Loaded:" << filebytes.size() << "chars from config file";
	file.close();
	return parseMetaData(filebytes);
}

const Table3DMetaData FEMemoryMetaData::get3DMetaData(unsigned short locationid)
{
	for (int i=0;i<m_table3DMetaData.size();i++)
	{
		if (m_table3DMetaData[i].locationId == locationid)
		{
			return m_table3DMetaData[i];
		}
	}
	return Table3DMetaData();
}


const Table2DMetaData FEMemoryMetaData::get2DMetaData(unsigned short locationid)
{
	for (int i=0;i<m_table2DMetaData.size();i++)
	{
		if (m_table2DMetaData[i].locationId == locationid)
		{
			return m_table2DMetaData[i];
		}
	}
	return Table2DMetaData();
}
bool FEMemoryMetaData::has3DMetaData(unsigned short locationid)
{
	for (int i=0;i<m_table3DMetaData.size();i++)
	{
		if (m_table3DMetaData[i].locationId == locationid)
		{
			return true;
		}
	}
	return false;
}

bool FEMemoryMetaData::has2DMetaData(unsigned short locationid)
{
	for (int i=0;i<m_table2DMetaData.size();i++)
	{
		if (m_table2DMetaData[i].locationId == locationid)
		{
			return true;
		}
	}
	return false;
}
bool FEMemoryMetaData::hasRORMetaData(unsigned short locationid)
{
	for (int i=0;i<m_readOnlyMetaData.size();i++)
	{
		if (m_readOnlyMetaData[i].locationId == locationid)
		{
			return true;
		}
	}
	return false;
}
bool FEMemoryMetaData::hasLookupMetaData(unsigned short locationid)
{
	return (m_lookupMetaData.contains(locationid));
}

const LookupMetaData FEMemoryMetaData::getLookupMetaData(unsigned short locationid)
{
	if (m_lookupMetaData.contains(locationid))
	{
		return m_lookupMetaData[locationid];
	}
	return LookupMetaData();
}

const ReadOnlyRamData FEMemoryMetaData::getRORMetaData(unsigned short locationid)
{
	for (int i=0;i<m_readOnlyMetaData.size();i++)
	{
		if (m_readOnlyMetaData[i].locationId == locationid)
		{
			return m_readOnlyMetaData[i];
		}
	}
	return ReadOnlyRamData();
}

const QString FEMemoryMetaData::getErrorString(unsigned short code)
{
	if (m_errorMap.contains(code))
	{
		return m_errorMap[code];
	}
	return "0x" + QString::number(code,16).toUpper();
}
