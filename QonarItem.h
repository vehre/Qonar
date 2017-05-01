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
#ifndef QONARITEM_H
#define QONARITEM_H
#include "Constants.h"
#include <QColor>
#include <QIcon>
#include <QMetaType>
#include <QString>
#include <utils/fileutils.h>
#include <vector>
namespace Qonar
{
    namespace Internal
    {
        class QonarItem
        {
            public:
                QonarItem():line(-1),color(Qt::transparent) {}

                QString                 text;
                QString                 user;
                QString                 status;
                Utils::FileName         file;
                int                     line;
                Constants::IssueType    issueType;
                QColor                  color;

                QIcon icon();
                static QIcon toIcon(Constants::IssueType issueType);
        };

        class QonarItemSortPredicate
        {
            public:
                explicit QonarItemSortPredicate(Constants::OutputColumnIndex    columnIndex,
                                                Qt::SortOrder                   order):
                    m_columnIndex(columnIndex),
                    m_order(order)
                {}

                inline bool operator()(const QonarItem& t1,
                                       const QonarItem& t2)
                {
                    if(m_order == Qt::AscendingOrder) return lessThan(t1, t2);
                    else return lessThan(t2, t1);
                }

                inline bool lessThan(const QonarItem&   t1,
                                     const QonarItem&   t2)
                {
                    switch(m_columnIndex)
                    {
                    case Constants::Type:
                        return t1.issueType < t2.issueType;

                    case Constants::User:
                        return t1.user < t2.user;

                    case Constants::Text:
                        return t1.text < t2.text;

                    case Constants::Status:
                        return t1.status < t2.status;

                    case Constants::File:
                        return t1.file < t2.file;

                    default:
                        return false;
                    } // switch
                } // lessThan

            private:
                Constants::OutputColumnIndex    m_columnIndex;
                Qt::SortOrder                   m_order;
        };
    } // namespace Internal
} // namespace Qonar
Q_DECLARE_METATYPE(Qonar::Internal::QonarItem)
#endif // QONARITEM_H
