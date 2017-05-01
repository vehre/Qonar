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
#include "QonarOutputPane.h"
#include <QButtonGroup>
#include <QHeaderView>
#include <QIcon>
#include <QMetaEnum>
#include <QSortFilterProxyModel>
#include <QToolButton>
#include <QTreeView>
#include <QMessageBox>
#include <aggregation/aggregate.h>
#include <coreplugin/find/itemviewfind.h>
#include <utils/icon.h>
#include <utils/utilsicons.h>
#include <coreplugin/icore.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

#include "Constants.h"
#include "QonarItem.h"
#include "QonarItemProvider.h"
#include "QonarModel.h"
#include "QonarOutputTreeView.h"
#include "QonarSettings.h"

namespace Qonar
{
    namespace Internal
    {
        QonarOutputPane::QonarOutputPane(QObject* parent):
            Core::IOutputPane(parent)
        {
            m_qonarItemProvider = new QonarItemProvider(this);
            createTreeView();
            createScopeButtons();
            ProjectExplorer::ProjectTree* tree = ProjectExplorer::ProjectTree::instance();

            // TODO only when active procgect is changed ?
            connect(tree,
                     SIGNAL(currentProjectChanged(ProjectExplorer::Project*)),
                     m_qonarItemProvider,
                     SLOT(refreshSilent()));
        }

        QonarOutputPane::~QonarOutputPane()
        {
            freeTreeView();
            freeScopeButtons();
        }

        QWidget* QonarOutputPane::outputWidget(QWidget* parent)
        {
            Q_UNUSED(parent)
            return m_qonarTreeView;
        }

        QList<QWidget*>QonarOutputPane::toolBarWidgets() const
        {
            QWidgetList widgets;

            for(QToolButton* btn : m_filterButtons) widgets << btn;

            widgets << m_spacer << m_refreshButton << m_configureButton;

            return widgets;
        }

        QString QonarOutputPane::displayName() const
        {
            return QT_TRANSLATE_NOOP("Qonar::Internal::QonarOutputPane", QString::fromUtf8("Sonar Entries"));
        }

        int QonarOutputPane::priorityInStatusBar() const
        {
            return 1;
        }

        void QonarOutputPane::clearContents()
        {}

        void QonarOutputPane::visibilityChanged(bool visible)
        {
            Q_UNUSED(visible)
        }

        void QonarOutputPane::setFocus()
        {
            m_qonarTreeView->setFocus();
        }

        bool QonarOutputPane::hasFocus() const
        {
            return m_qonarTreeView->window()->focusWidget() == m_qonarTreeView;
        }

        bool QonarOutputPane::canFocus() const
        {
            return true;
        }

        bool QonarOutputPane::canNavigate() const
        {
            return true;
        }

        bool QonarOutputPane::canNext() const
        {
            return m_qonarTreeView->model()->rowCount() > 0;
        }

        bool QonarOutputPane::canPrevious() const
        {
            return m_qonarTreeView->model()->rowCount() > 0;
        }

        void QonarOutputPane::goToNext()
        {
            const QModelIndex nextIndex = nextModelIndex();

            m_qonarTreeView->selectionModel()->setCurrentIndex(nextIndex, QItemSelectionModel::SelectCurrent
                                                               | QItemSelectionModel::Rows | QItemSelectionModel::Clear);
            qonarTreeViewClicked(nextIndex);
        }

        void QonarOutputPane::goToPrev()
        {
            const QModelIndex prevIndex = previousModelIndex();

            m_qonarTreeView->selectionModel()->setCurrentIndex(prevIndex, QItemSelectionModel::SelectCurrent
                                                               | QItemSelectionModel::Rows | QItemSelectionModel::Clear);
            qonarTreeViewClicked(prevIndex);
        }

        void QonarOutputPane::editSettings()
        {

            ProjectExplorer::ProjectTree* tree = ProjectExplorer::ProjectTree::instance();

            if(!tree){
                QMessageBox::warning(Core::ICore::mainWindow(),
                                        tr( "No project tree"),
                                        tr( "No project tree available. "));
                return;
            }

            ProjectExplorer::Project* current_project = tree->currentProject();
            if(!current_project){
                QMessageBox::warning(Core::ICore::mainWindow(),
                                        tr( "No project"),
                                        tr( "No current project available. "));
                return;
            }
            QString pUrl = current_project->namedSettings("qonar_url").toString();
            QString pProject = current_project->namedSettings("qonar_project").toString();

            QonarSettings* settings = new QonarSettings(Core::ICore::mainWindow());
            settings->setUrl(pUrl);
            settings->setProject(pProject);
            settings->show();
            if(settings->exec() == QDialog::Accepted)
            {
                current_project->setNamedSettings("qonar_url", settings->url());
                current_project->setNamedSettings("qonar_project", settings->project());
            }
            delete settings;
        }

        void QonarOutputPane::createTreeView()
        {
            m_qonarItemsModel = m_qonarItemProvider->getItemsModel();
            m_filteredQonarItemsModel = new QSortFilterProxyModel();
            m_filteredQonarItemsModel->setSourceModel(m_qonarItemsModel);
            m_filteredQonarItemsModel->setDynamicSortFilter(false);
            m_filteredQonarItemsModel->setFilterKeyColumn(Constants::Type);

            m_qonarTreeView = new QonarOutputTreeView();
            m_qonarTreeView->setModel(m_filteredQonarItemsModel);
            Aggregation::Aggregate* agg = new Aggregation::Aggregate;
            agg->add(m_qonarTreeView);
            agg->add(new Core::ItemViewFind(m_qonarTreeView));

            connect(m_qonarTreeView, &QTreeView::activated, this, &QonarOutputPane::qonarTreeViewClicked);
        }

        void QonarOutputPane::freeTreeView()
        {
            delete m_qonarTreeView;
            delete m_filteredQonarItemsModel;
        }

        QToolButton* QonarOutputPane::createCheckableToolButton(const QString&  text,
                                                                const QString&  toolTip,
                                                                const QIcon&    icon)
        {
            QToolButton* button = new QToolButton();

            button->setCheckable(true);
            button->setText(text);
            button->setToolTip(toolTip);
            button->setIcon(icon);

            return button;
        }

        void QonarOutputPane::createScopeButtons()
        {
            m_refreshButton = createCheckableToolButton("Refresh",
                                                        "Refresh Issues",
                                                        Utils::Icons::RELOAD.icon());
            m_refreshButton->setCheckable(false);
            m_refreshButton->connect(m_refreshButton,
                                     SIGNAL(clicked()),
                                     m_qonarItemProvider,
                                     SLOT(refreshVerbose()));

            m_configureButton = createCheckableToolButton("Settings",
                                                          "Edit Qonar settings for the current Project",
                                                          QIcon::fromTheme("preferences-system"));
            m_configureButton->setCheckable(false);
            m_refreshButton->connect(m_configureButton,
                                     SIGNAL(clicked()),
                                     this,
                                     SLOT(editSettings()));

            // m_wholeProjectButton = new QToolButton();
            // m_wholeProjectButton->setCheckable(true);
            // m_wholeProjectButton->setText(tr("Active Project"));
            // m_wholeProjectButton->setToolTip(tr("Scan the whole active project."));

            // m_subProjectButton = new QToolButton();
            // m_subProjectButton->setCheckable(true);
            // m_subProjectButton->setText(tr("Subproject"));
            // m_subProjectButton->setToolTip(tr("Scan the current subproject."));

            // m_scopeButtons = new QButtonGroup();
            // m_scopeButtons->addButton(m_wholeProjectButton);
            // m_scopeButtons->addButton(m_currentFileButton);
            // m_scopeButtons->addButton(m_subProjectButton);
            // connect(m_scopeButtons, static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
            // this, &QonarOutputPane::scopeButtonClicked);

            m_spacer = new QWidget;
            m_spacer->setMinimumWidth(25);

            QString     tooltip = tr("Show \"%1\" issues");
            QMetaEnum   type = QMetaEnum::fromType<Constants::IssueType>();
            for(int i = 0; i < type.keyCount(); i++)
            {
                QString         name = QString::fromUtf8(type.valueToKey(i));
                QToolButton*    button = createCheckableToolButton(name, tooltip.arg(name), QonarItem::toIcon((Constants::IssueType)i));
                button->setProperty("type", name);
                button->setToolButtonStyle(Qt::ToolButtonIconOnly);
                connect(button, &QToolButton::clicked, this, &QonarOutputPane::updateFilter);

                m_filterButtons.append(button);
            }
        } // createScopeButtons

        void QonarOutputPane::freeScopeButtons()
        {
            // delete m_currentFileButton;
            // delete m_wholeProjectButton;
            // delete m_subProjectButton;
            // delete m_scopeButtons;
            delete m_spacer;
            delete m_refreshButton;
            delete m_configureButton;
            qDeleteAll(m_filterButtons);
        }

        void QonarOutputPane::qonarTreeViewClicked(const QModelIndex& index)
        {
            emit qonarItemClicked(index.data(Qt::UserRole).value<QonarItem>());
        }

        void QonarOutputPane::updateQonarCount()
        {
            setBadgeNumber(m_qonarTreeView->model()->rowCount());
        }

        void QonarOutputPane::updateFilter()
        {
            QStringList keywords;

            for(QToolButton* btn : m_filterButtons)
            {
                if(btn->isChecked()) keywords.append(btn->property("type").toString());
            }

            QString         pattern = keywords.isEmpty()?QString():QString("^(%1).*").arg(keywords.join('|'));
            int             sortColumn = m_qonarTreeView->header()->sortIndicatorSection();
            Qt::SortOrder   sortOrder = m_qonarTreeView->header()->sortIndicatorOrder();

            // TODO Sort by severity...
            m_filteredQonarItemsModel->setFilterRegExp(pattern);
            m_filteredQonarItemsModel->sort(sortColumn, sortOrder);

            updateQonarCount();
        } // updateFilter

        void QonarOutputPane::clearFilter()
        {
            for(QToolButton* btn : m_filterButtons) btn->setChecked(false);

            updateFilter();
        }

        QModelIndex QonarOutputPane::selectedModelIndex()
        {
            QModelIndexList selectedIndexes = m_qonarTreeView->selectionModel()->selectedIndexes();

            if(selectedIndexes.isEmpty()) return QModelIndex();
            else
                // There is only one item selected
                return selectedIndexes.first();
        }

        QModelIndex QonarOutputPane::nextModelIndex()
        {
            QModelIndex indexToBeSelected = m_qonarTreeView->indexBelow(selectedModelIndex());

            if(!indexToBeSelected.isValid()) return m_qonarTreeView->model()->index(0, 0);
            else return indexToBeSelected;
        }

        QModelIndex QonarOutputPane::previousModelIndex()
        {
            QModelIndex indexToBeSelected = m_qonarTreeView->indexAbove(selectedModelIndex());

            if(!indexToBeSelected.isValid()) return m_qonarTreeView->model()->index(m_qonarTreeView->model()->rowCount() - 1, 0);
            else return indexToBeSelected;
        }
    } // namespace Internal
} // namespace Qonar
