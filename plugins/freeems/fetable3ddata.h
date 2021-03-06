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

#ifndef FETABLE3DDATA_H
#define FETABLE3DDATA_H

#include <QObject>
#include "headers.h"
#include "table3ddata.h"
class FETable3DData : public Table3DData
{
	Q_OBJECT
public:
	FETable3DData();
	//FETable3DData(unsigned short locationid,bool isflashonly, QByteArray data,Table3DMetaData metadata);
	void setData(unsigned short locationid,bool isflashonly,QByteArray payload,Table3DMetaData metadata);
	void setData(unsigned short locationid,bool isflashonly,QByteArray payload);
	QByteArray data();
	QList<double> xAxis();
	QList<double> yAxis();
	QList<QList<double> > values();
	int columns();
	int rows();
	void setCell(int row, int column,double val);
	void setXAxis(int column,double val);
	void setYAxis(int row,double val);
	double maxXAxis();
	double maxYAxis();
	double maxZAxis();
	double minXAxis();
	double minYAxis();
	double minZAxis();
	void setWritesEnabled(bool enabled);
	void writeWholeLocation();
private:
	bool m_writesEnabled;
	bool m_isFlashOnly;
	unsigned short m_locationId;
	QList<double> m_xAxis;
	QList<double> m_yAxis;
	QList<QList<double> > m_values;
	QString xAxisLabel;
	QString yAxisLabel;
	QString valuesLabel;
	double m_maxXAxis;
	double m_maxYAxis;
	double m_maxZAxis;
	double m_minXAxis;
	double m_minYAxis;
	double m_minZAxis;
	Table3DMetaData m_metaData;
signals:
	void saveSingleData(unsigned short locationid,QByteArray data, unsigned short offset, unsigned short size);
public slots:
	
};

#endif // FETABLE3DDATA_H
