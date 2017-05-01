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
#include "QonarOutputTreeView.h"
#include "Constants.h"

#include <coreplugin/icore.h>

#include <QHeaderView>
#include <QResizeEvent>
#include <QSettings>

namespace Qonar
{
    namespace Internal
    {
        QonarOutputTreeView::QonarOutputTreeView(QWidget* parent):
            Utils::TreeView(parent),
            m_textColumnDefaultWidth(0),
            m_fileColumnDefaultWidth(0)
        {
            setRootIsDecorated(false);
            setFrameStyle(QFrame::NoFrame);
            setSortingEnabled(true);
            setAttribute(Qt::WA_MacShowFocusRect, false);
            setSelectionBehavior(QTreeView::SelectRows);

            // setItemDelegate(new QonarOutputTreeViewDelegate(this));

            header()->setSectionResizeMode(QHeaderView::Interactive);
            header()->setStretchLastSection(true);
            header()->setSectionsMovable(false);
            connect(header(), &QHeaderView::sectionResized, this, &QonarOutputTreeView::qonarColumnResized);
            loadDisplaySettings();
        }

        QonarOutputTreeView::~QonarOutputTreeView()
        {
            saveDisplaySettings();
        }

        void QonarOutputTreeView::saveDisplaySettings()
        {
//            QSettings* settings = Core::ICore::settings();

//            settings->beginGroup(QLatin1String(Constants::SETTINGS_GROUP));
//            settings->setValue(QLatin1String(Constants::OUTPUT_PANE_TEXT_WIDTH),
//                                columnWidth(Constants::OUTPUT_COLUMN_TEXT));
//            settings->setValue(QLatin1String(Constants::OUTPUT_PANE_FILE_WIDTH),
//                                columnWidth(Constants::OUTPUT_COLUMN_FILE));
//            settings->endGroup();
        }

        void QonarOutputTreeView::loadDisplaySettings()
        {
//            QSettings* settings = Core::ICore::settings();

//            settings->beginGroup(QLatin1String(Constants::SETTINGS_GROUP));
//            m_textColumnDefaultWidth = settings->value(
//                QLatin1String(  Constants::OUTPUT_PANE_TEXT_WIDTH), 0).toInt();
//            m_fileColumnDefaultWidth = settings->value(
//                QLatin1String(  Constants::OUTPUT_PANE_FILE_WIDTH), 0).toInt();
//            settings->endGroup();
        }

        void QonarOutputTreeView::resizeEvent(QResizeEvent* event)
        {
            int widthText = m_textColumnDefaultWidth;
            int widthFile = m_fileColumnDefaultWidth;

            if((event->oldSize().width() == 0) || (event->oldSize().width() == -1))
            {
                if(widthText == 0) widthText = 0.55 * event->size().width();
                if(widthFile == 0) widthFile = 0.35 * event->size().width();
            } else {
                const qreal scale = static_cast<qreal>(event->size().width())
                                    / static_cast<qreal>(event->oldSize().width());
                widthText = scale * columnWidth(Constants::Text);
                widthFile = scale * columnWidth(Constants::File);
            }

            setColumnWidth( Constants::Text,  widthText);
            setColumnWidth( Constants::File,  widthFile);
        } // resizeEvent

        void QonarOutputTreeView::qonarColumnResized(int    column,
                                                     int    oldSize,
                                                     int    newSize)
        {
            Q_UNUSED(oldSize);
            if(column == Constants::Text) m_textColumnDefaultWidth = newSize;
            else if(column == Constants::File) m_fileColumnDefaultWidth = newSize;
        }
    } // namespace Internal
} // namespace Qonar
