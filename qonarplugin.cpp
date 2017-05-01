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
#include "qonarplugin.h"
#include "QonarOutputPane.h"
#include "QonarItem.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

namespace Qonar
{
    namespace Internal
    {
        QonarPlugin::QonarPlugin()
        {
            // Create your members
        }

        QonarPlugin::~QonarPlugin()
        {
            // Unregister objects from the plugin manager's object pool
            // Delete members
        }

        bool
        QonarPlugin::initialize(const QStringList&  arguments,
                                QString*            errorString)
        {
            // Register objects in the plugin manager's object pool
            // Load settings
            // Add actions to menus
            // Connect to other plugins' signals
            // In the initialize function, a plugin can be sure that the plugins it
            // depends on have initialized their members.

            Q_UNUSED(   arguments)
            Q_UNUSED(   errorString)

            createQonarOutputPane();
            return true;

            // QAction *action = new QAction(tr("Qonar Action"), this);
            // Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
            // Core::Context(Core::Constants::C_GLOBAL));
            // cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
            // connect(action, &QAction::triggered, this, &QonarPlugin::triggerAction);

            // Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
            // menu->menu()->setTitle(tr("Qonar"));
            // menu->addAction(cmd);
            // Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

            // return true;
        } // initialize

        void
        QonarPlugin::extensionsInitialized()
        {
            // Retrieve objects from the plugin manager's object pool
            // In the extensionsInitialized function, a plugin can be sure that all
            // plugins that depend on it are completely initialized.
        }

        ExtensionSystem::IPlugin::ShutdownFlag
        QonarPlugin::aboutToShutdown()
        {
            // Save settings
            // Disconnect from signals that are not needed during shutdown
            // Hide UI (if you add UI that is not in the main window directly)
            return SynchronousShutdown;
        }

        void QonarPlugin::qonarItemClicked(const QonarItem & item)
        {
            if (item.file.exists())
                Core::EditorManager::openEditorAt(item.file.toString(), item.line);
        }

        void
        QonarPlugin::createQonarOutputPane()
        {
            m_qonarPane = new QonarOutputPane();
            addAutoReleasedObject(m_qonarPane);
            connect(m_qonarPane, &QonarOutputPane::qonarItemClicked,
                    this, &QonarPlugin::qonarItemClicked);
        }
    } // namespace Internal
} // namespace Qonar
