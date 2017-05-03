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
#include "QonarItemProvider.h"
#include "QonarModel.h"

#include <QApplication>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QtDebug>
#include <coreplugin/icore.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

namespace Qonar
{
    namespace Internal
    {
        const QString                               QonarItemProvider::m_project_arg = "projectKeys";
        const QString                               QonarItemProvider::m_resolved_arg = "resolved";
        const QString                               QonarItemProvider::m_resolved_value = "false";
        const QString                               QonarItemProvider::m_api_issue = "api/issues/search";
        const QHash<QString, Constants::IssueType>  QonarItemProvider::m_sonarToQonar {
            {
                "INFO", Constants::Info
            },
            {
                "MINOR", Constants::Minor
            },
            {
                "MAJOR", Constants::Major
            },
            {
                "CRITICAL", Constants::Critical
            },
            {
                "BLOCKER", Constants::Blocker
            },
        };

        QonarItemProvider::QonarItemProvider(QObject* parent):
            QObject(parent),
            p_nam(new QNetworkAccessManager(this))
        {
            m_itemsModel = new QonarModel(this);
            m_itemsModel->setQonarItemsList(&m_itemList);

            connect(p_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(refreshDone(QNetworkReply*)));
        }

        void QonarItemProvider::setUrl(const QString& url)
        {
            setUrl(QUrl(url));
        }

        void QonarItemProvider::setUrl(const QUrl& url)
        {
            setScheme(url.scheme());
            setServer(url.host());
            setServer_path(url.path());
            setServer_port(url.port());
        }

        QString QonarItemProvider::server() const
        {
            return m_server;
        }

        void QonarItemProvider::setServer(const QString& server)
        {
            m_server = server;
        }

        int QonarItemProvider::server_port() const
        {
            return m_server_port;
        }

        void QonarItemProvider::setServer_port(int server_port)
        {
            m_server_port = server_port;
        }

        QString QonarItemProvider::server_path() const
        {
            return m_server_path;
        }

        void QonarItemProvider::setServer_path(const QString& server_path)
        {
            m_server_path = server_path;
        }

        QString QonarItemProvider::project() const
        {
            return m_project;
        }

        void QonarItemProvider::setProject(const QString& project)
        {
            m_project = project;
        }

        void QonarItemProvider::refresh(bool silent)
        {
            ProjectExplorer::ProjectTree* tree = ProjectExplorer::ProjectTree::instance();

            if(!tree) return;

            ProjectExplorer::Project* current_project = tree->currentProject();
            if(!current_project) return;

            m_projectDir = current_project->projectDirectory().toString();

            QString pUrl = current_project->namedSettings("qonar_url").toString();
            QString pProject = current_project->namedSettings("qonar_project").toString();
            if(pUrl.isEmpty() || pProject.isEmpty())
            {
                m_itemList.clear();
                m_itemsModel->qonarItemsListUpdated();
                if(!silent)
                {
                    QMessageBox::warning(Core::ICore::mainWindow(),
                                            tr( "No configuration"),
                                            tr( "Qonar is not configured for this project. "));
                }
                return;
            }
            setUrl(pUrl);
            setProject(pProject);

            QUrl url;

            url.setScheme(scheme());
            url.setHost(server());
            url.setPort(server_port());
            url.setPath(server_path() + "/" + m_api_issue);
            QUrlQuery query;
            query.addQueryItem(m_project_arg, project());
            query.addQueryItem(m_resolved_arg, m_resolved_value);
            url.setQuery(query);
                    getUrl(url);
        } // refresh

        void QonarItemProvider::refreshDone(QNetworkReply* reply)
        {
            if(reply->error() == QNetworkReply::NoError)
            {
                int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                if((status == 301) || (status == 302))
                {
                    getUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
                } else {
                    // Read the result
                    QJsonDocument   response = QJsonDocument::fromJson(reply->readAll());
                    QJsonObject     object = response.object();
                    QJsonArray      jsonIssues = object["issues"].toArray();
                    m_itemList.clear();
                    for(const QJsonValue& val : jsonIssues)
                    {
                        QJsonObject issue = val.toObject();
                        QonarItem   item;
                        item.issueType = m_sonarToQonar[issue["severity"].toString()];
                        item.status = issue["status"].toString();
                        item.user = issue["author"].toString();
                        item.text = issue["message"].toString();
                        QStringList file = issue["component"].toString().split(":");
                        item.file = Utils::FileName::fromString(m_projectDir + QDir::separator() + file.last());
                        item.line = issue["line"].toInt();
                        m_itemList.append(item);
                    }
                    m_itemsModel->qonarItemsListUpdated();
                }
            } else {
                // TODO error handling
                qDebug() << "ERROR" << reply->error();
            }
        } // refreshDone

        void QonarItemProvider::getUrl(const QUrl& url)
        {
                qDebug() << url;
            p_nam->get(QNetworkRequest(url));
        }

        QonarModel* QonarItemProvider::getItemsModel() const
        {
            return m_itemsModel;
        }

        void QonarItemProvider::refreshSilent()
        {
            refresh(true);
        }

        void QonarItemProvider::refreshVerbose()
        {
            refresh(false);
        }

        void QonarItemProvider::setScheme(const QString& scheme)
        {
            m_scheme = scheme;
        }

        QString QonarItemProvider::scheme() const
        {
            return m_scheme;
        }
    } // namespace Internal
} // namespace Qonar
