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
#include "QonarModel.h"

#include <QMetaEnum>
#include <utils/algorithm.h>

#include <utils/theme/theme.h>

namespace Qonar
{
    namespace Internal
    {
        QonarModel::QonarModel(QObject* parent):
            QAbstractTableModel(parent),
            m_qonarItemsList(nullptr),
            m_currentSortColumn(Constants::OutputColumnIndex(0)),
            m_currentSortOrder(Qt::AscendingOrder)
        {
            m_qonarItemsList = new QList<QonarItem>();
            QMetaEnum type = QMetaEnum::fromType<Constants::IssueType>();
            for(int i = 0; i < type.keyCount(); i++)
            {
                QString     name = QString::fromUtf8(type.valueToKey(i));
                QonarItem   item;
                item.issueType = (Constants::IssueType)i;
                item.text = name;
                m_qonarItemsList->append(item);
            }
        }

        void QonarModel::setQonarItemsList(QList<QonarItem>* list)
        {
            m_qonarItemsList = list;
            qonarItemsListUpdated();
        }

        int QonarModel::rowCount(const QModelIndex& parent) const
        {
            Q_UNUSED(parent)

            // There's only one level of hierarchy
            if(parent.isValid()) return 0;

            if(!m_qonarItemsList) return 0;

            return m_qonarItemsList->count();
        }

        int QonarModel::columnCount(const QModelIndex& parent) const
        {
            Q_UNUSED(parent)

            return Constants::Count;
        }

        QVariant QonarModel::data(const QModelIndex&    index,
                                  int                   role) const
        {
            QVariant res;

            if(index.isValid() and m_qonarItemsList)
            {
                if(role == Qt::UserRole)
                {
                    res = QVariant::fromValue(m_qonarItemsList->at(index.row()));
                } else {
                    QonarItem item = index.data(Qt::UserRole).value<QonarItem>();

                    if(role == Qt::BackgroundColorRole) res = item.color;

                    if(role == Qt::TextColorRole) res = Utils::creatorTheme()->color(Utils::Theme::MenuItemTextColorNormal);

                    if(role == Qt::ForegroundRole) res = Utils::creatorTheme()->color(Utils::Theme::MenuItemTextColorNormal);

                    if(role == Qt::DisplayRole)
                        switch(Constants::OutputColumnIndex(index.column()))
                        {
                        case Constants::User:
                            res = item.user;

                            break;
                        case Constants::Text:
                            res = item.text;

                            break;
                        case Constants::Type:
                            {
                            QMetaEnum type = QMetaEnum::fromType<Constants::IssueType>();
                            res = type.valueToKey(item.issueType);

                            break;
                            }
                        case Constants::Status:
                            res = item.status;

                            break;

                        case Constants::File:
                            res = item.file.toUserOutput();

                            break;

                        case Constants::Line:
                            res = item.line;

                            break;
                        default:
                            break;
                        } // switch
                    // switch

                    if((role == Qt::DecorationRole) && (index.column() == Constants::Type)) res = item.icon();
                }
            }
            return res;
        } // data

        QVariant QonarModel::headerData(int             section,
                                        Qt::Orientation orientation,
                                        int             role) const
        {
            QVariant res;

            if((orientation != Qt::Vertical) && (role == Qt::DisplayRole))
                switch(Constants::OutputColumnIndex(section))
                {
                case Constants::Type:
                    res = tr("Severity");
                    break;
                case Constants::User:
                    res = tr("Author");
                    break;
                case Constants::Status:
                    res = tr("Status");
                    break;
                case Constants::Text:
                    res = tr("Description");
                    break;
                case Constants::File:
                    res = tr("File");
                    break;
                case Constants::Line:
                    res = tr("Line");
                    break;

                default:
                    return QVariant();
                } // switch
            // switch
            // switch

            return res;
        } // headerData

        void QonarModel::sort(int           column,
                              Qt::SortOrder order)
        {
            if(!m_qonarItemsList) return;

            m_currentSortColumn = Constants::OutputColumnIndex(column);
            m_currentSortOrder = order;

            QonarItemSortPredicate  predicate(m_currentSortColumn, m_currentSortOrder);
            emit                    layoutAboutToBeChanged();
            Utils::sort(*m_qonarItemsList, predicate);
            emit layoutChanged();
        }

        void QonarModel::qonarItemsListUpdated()
        {
            if(!m_qonarItemsList) return;

            sort(m_currentSortColumn, m_currentSortOrder);
        }
    } // namespace Internal
} // namespace Qonar
