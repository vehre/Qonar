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
#include "QonarSettings.h"
#include "ui_QonarSettings.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include "QonarSonar.h"

namespace Qonar
{
    namespace Internal
    {
        QonarSettings::QonarSettings(QonarSonar* qonarSonar, QWidget* parent):
            QDialog(parent),
            ui(new Ui::QonarSettings),
            ptr_qonarSonar(qonarSonar)
        {
            ui->setupUi(this);
            connect(ui->lineEdit,   SIGNAL(editingFinished()),          this,   SLOT(refreshProjects()));
            readSettings();
        }

        QonarSettings::~QonarSettings()
        {
            delete ui;
        }

        void QonarSettings::refreshProjects()
        {
            updateSettings();
            ui->comboBox->clear();
            m_projectsKeys.clear();
            QMap<QString, QString> projects = ptr_qonarSonar->projects();
            for(QString key : projects.keys())
            {
                ui->comboBox->addItem(projects[key]);
                m_projectsKeys.append(key);
            }
        }

        void QonarSettings::accept()
        {
            updateSettings();
            QDialog::accept();
        }

        void QonarSettings::reject()
        {
            ProjectExplorer::ProjectTree* tree = ProjectExplorer::ProjectTree::instance();
            if(!tree) return;
            ProjectExplorer::Project* current_project = tree->currentProject();
            current_project->setNamedSettings(QonarSonar::m_qonar_url_settings, m_orig_url);
            current_project->setNamedSettings(QonarSonar::m_qonar_project_settings, m_orig_project);
            QDialog::reject();
        }

        void QonarSettings::readSettings()
        {
            ProjectExplorer::ProjectTree* tree = ProjectExplorer::ProjectTree::instance();
            if(!tree) return;
            ProjectExplorer::Project* current_project = tree->currentProject();
            if(!current_project) return;
            m_orig_project = current_project->namedSettings(QonarSonar::m_qonar_project_settings).toString();
            m_orig_url = current_project->namedSettings(QonarSonar::m_qonar_url_settings).toString();
            ui->lineEdit->setText(m_orig_url);
            int index = m_projectsKeys.indexOf(m_orig_project);
            ui->comboBox->setCurrentIndex(index);
        }

        void QonarSettings::updateSettings()
        {
            ProjectExplorer::ProjectTree* tree = ProjectExplorer::ProjectTree::instance();
            if(!tree) return;
            ProjectExplorer::Project* current_project = tree->currentProject();
            if(!current_project) return;
            current_project->setNamedSettings(QonarSonar::m_qonar_url_settings, ui->lineEdit->text());
            int index = ui->comboBox->currentIndex();
            if(m_projectsKeys.size() > 0 && index >= 0)
                current_project->setNamedSettings(QonarSonar::m_qonar_project_settings, m_projectsKeys[index]);
            current_project->saveSettings();
        }
    } // namespace Internal
} // namespace Qonar
