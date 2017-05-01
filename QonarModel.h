/*
 * This file is part of the Qonar QtCreator plugin.
 *
 *     Qonar is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     Foobar is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with Qonar.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef QONARMODEL_H
#define QONARMODEL_H

#include "Constants.h"
#include "QonarItem.h"
#include <QAbstractTableModel>
#include <QList>

namespace Qonar
{
    namespace Internal
    {
        class QonarModel:public QAbstractTableModel
        {
            Q_OBJECT

            public:
                explicit QonarModel(QObject* parent);
                void setQonarItemsList(QList<QonarItem>* list);

                int rowCount(const QModelIndex& parent = QModelIndex()) const;

                int columnCount(const QModelIndex& parent = QModelIndex()) const;

                QVariant data(const QModelIndex&    index,
                              int                   role) const;

                QVariant headerData(int             section,
                                    Qt::Orientation orientation,
                                    int             role) const;

                void sort(int           column,
                          Qt::SortOrder order = Qt::AscendingOrder);

                void qonarItemsListUpdated();

            private:
                QList<QonarItem>*               m_qonarItemsList;
                Constants::OutputColumnIndex    m_currentSortColumn;
                Qt::SortOrder                   m_currentSortOrder;
        };
    } // namespace Internal
} // namespace Qonar

#endif // QONARMODEL_H
