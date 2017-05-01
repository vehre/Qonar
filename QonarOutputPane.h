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
#ifndef QONAROUTPUTPANE_H
#define QONAROUTPUTPANE_H

#include <coreplugin/ioutputpane.h>

QT_BEGIN_NAMESPACE
class QToolButton;
class QButtonGroup;
class QModelIndex;
class QAbstractButton;
class QSortFilterProxyModel;
class QAbstractItemModel;
QT_END_NAMESPACE

class QTreeView;
namespace Qonar
{
    namespace Internal
    {
        typedef QList<QToolButton*>QToolButtonList;
        class QonarModel;
        class QonarItem;
        class QonarItemProvider;
        class QonarOutputPane:public Core::IOutputPane
        {
            Q_OBJECT

            public:
                QonarOutputPane(QObject* parent = 0);
                virtual ~QonarOutputPane();
                QWidget* outputWidget(QWidget* parent);

                QList<QWidget*>toolBarWidgets() const;

                QString displayName() const;

                int priorityInStatusBar() const;

                void clearContents();

                void visibilityChanged(bool visible);

                void setFocus();

                bool hasFocus() const;

                bool canFocus() const;

                bool canNavigate() const;

                bool canNext() const;

                bool canPrevious() const;

                void goToNext();

                void goToPrev();

            signals:
                void qonarItemClicked(const QonarItem& item);

            private slots:
                void editSettings();

            private:
                void createTreeView();

                void freeTreeView();

                void createScopeButtons();

                void freeScopeButtons();

                void qonarTreeViewClicked(const QModelIndex& index);

                void updateQonarCount();

                void updateFilter();

                void clearFilter();

                QModelIndex selectedModelIndex();

                QModelIndex nextModelIndex();

                QModelIndex previousModelIndex();

                QToolButton* createCheckableToolButton(const QString&   text,
                                                       const QString&   toolTip,
                                                       const QIcon&     icon);

                QTreeView*              m_qonarTreeView;
                QonarModel*             m_qonarItemsModel;
                QonarItemProvider*      m_qonarItemProvider;
                QSortFilterProxyModel*  m_filteredQonarItemsModel;
                QToolButtonList         m_filterButtons;
                QToolButton*            m_refreshButton;
                QToolButton*            m_configureButton;
                QWidget*                m_spacer;
        };
    } // namespace Internal
} // namespace Qonar

#endif // QONAROUTPUTPANE_H
