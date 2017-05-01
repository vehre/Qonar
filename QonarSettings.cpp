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

namespace Qonar
{
    namespace Internal
    {
        const QString QonarSettings::m_api_project = "api/projects/index";
        QonarSettings::QonarSettings(QWidget* parent):
            QDialog(parent),
            ui(new Ui::QonarSettings),
            p_nam(new QNetworkAccessManager(this))
        {
            m_reply_finished = false;
            ui->setupUi(this);
            connect(ui->lineEdit,   SIGNAL(editingFinished()),          this,   SLOT(refreshProjects()));
            connect(p_nam,          SIGNAL(finished(QNetworkReply*)),   this,   SLOT(refreshProjectsDone(QNetworkReply*)));
        }

        QonarSettings::~QonarSettings()
        {
            delete ui;
        }

        void QonarSettings::setUrl(const QString& url)
        {
            ui->lineEdit->setText(url);
            refreshProjects();
            while(!m_reply_finished)
                qApp->processEvents();
        }

        QString QonarSettings::url() const
        {
            return ui->lineEdit->text();
        }

        void QonarSettings::setProject(const QString& projectKey)
        {
            int index = m_projectsKeys.indexOf(projectKey);

            ui->comboBox->setCurrentIndex(index);
        }

        QString QonarSettings::project() const
        {
            return m_projectsKeys[ui->comboBox->currentIndex()];
        }

        void QonarSettings::refreshProjects()
        {
            m_reply_finished = false;
            QUrl url(ui->lineEdit->text());
            QString path = url.path() + "/" + m_api_project;
            url.setPath(path);
            QUrlQuery query;
            query.addQueryItem("format", "json");
            url.setQuery(query);
                    getUrl(url);
        }

        void QonarSettings::refreshProjectsDone(QNetworkReply* reply)
        {
            ui->comboBox->clear();
            m_projectsKeys.clear();
            if(reply->error() == QNetworkReply::NoError)
            {
                int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                if((status == 301) || (status == 302))
                {
                    getUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
                } else {
                    // Read the result
                    QJsonDocument   response = QJsonDocument::fromJson(reply->readAll());
                    QJsonArray      projects = response.array();
                    for(const QJsonValue& val : projects)
                    {
                        QJsonObject project = val.toObject();
                        ui->comboBox->addItem(project["nm"].toString());
                        m_projectsKeys.append(project["k"].toString());
                    }
                    m_reply_finished = true;
                }
            } else {
                // TODO error handling
                qDebug() << "ERROR" << reply->error();
                m_reply_finished = true;
            }
        } // refreshProjectsDone

        void QonarSettings::getUrl(const QUrl& url)
        {
            qDebug() << url;
            p_nam->get(QNetworkRequest(url));
        }
    } // namespace Internal
} // namespace Qonar
