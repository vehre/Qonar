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
#ifndef QONAROUTPUTTREEVIEW_H
#define QONAROUTPUTTREEVIEW_H

#include <utils/itemviews.h>
namespace Qonar
{
    namespace Internal
    {
        class QonarOutputTreeView:public Utils::TreeView
        {
            public:
                explicit QonarOutputTreeView(QWidget* parent = 0);
                ~QonarOutputTreeView();

                void resizeEvent(QResizeEvent* event);

            private:
                void qonarColumnResized(int column,
                                        int oldSize,
                                        int newSize);

                void saveDisplaySettings();

                void loadDisplaySettings();

                qreal   m_textColumnDefaultWidth;
                qreal   m_fileColumnDefaultWidth;
        };
    } // namespace Internal
} // namespace Qonar
#endif // QONAROUTPUTTREEVIEW_H
