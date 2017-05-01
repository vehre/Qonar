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
#pragma once

#include "qonar_global.h"

#include <extensionsystem/iplugin.h>

namespace Qonar
{
    namespace Internal
    {
        class QonarOutputPane;
        class QonarItem;
        class QonarPlugin:public ExtensionSystem::IPlugin
        {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Qonar.json")

            public:QonarPlugin();
                ~QonarPlugin();

                bool initialize(const QStringList&  arguments,
                                QString*            errorString);

                void extensionsInitialized();

                ShutdownFlag aboutToShutdown();

            private:
                void qonarItemClicked(const QonarItem& item);

                void createQonarOutputPane();

                QonarOutputPane* m_qonarPane;
        };
    } // namespace Internal
} // namespace Qonar
