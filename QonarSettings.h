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
#ifndef QONARSETTINGS_H
#define QONARSETTINGS_H

#include <QDialog>

namespace Ui
{
    class QonarSettings;
}
class QNetworkAccessManager;
class QNetworkReply;
class QUrl;

namespace Qonar
{
    namespace Internal
    {
        class QonarSonar;

        class QonarSettings:public QDialog
        {
            Q_OBJECT

            public:
                explicit QonarSettings(QonarSonar* qonarSonar, QWidget* parent = 0);
                ~QonarSettings();

            private slots:
                void refreshProjects();
            public slots:
                virtual void accept();
                virtual void reject();
            private:
                void readSettings();
                void updateSettings();
                Ui::QonarSettings*      ui;
                QStringList             m_projectsKeys;
                QonarSonar*             ptr_qonarSonar;
                QString                 m_orig_url;
                QString                 m_orig_project;
        };
    } // namespace Internal
} // namespace Qonar

#endif // QONARSETTINGS_H
