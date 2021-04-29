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
#include "QonarSonar.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QInputDialog>

#include <coreplugin/icore.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

namespace Qonar
{
    namespace Internal
    {
        const QString                               QonarSonar::m_format_arg = "format";
        const QString                               QonarSonar::m_format_value = "json";
        const QString                               QonarSonar::m_api_issues = "api/issues/search";
        const QString                               QonarSonar::m_api_issues_project_arg = "projectKeys";
        const QString                               QonarSonar::m_api_issues_resolved_arg = "resolved";
        const QString                               QonarSonar::m_api_issues_resolved_value = "false";
        const QString                               QonarSonar::m_api_project = "api/projects/search";
        const QString                               QonarSonar::m_json_project_name = "name";
        const QString                               QonarSonar::m_json_project_key = "key";
        const QString                               QonarSonar::m_json_issues = "issues";
        const QString                               QonarSonar::m_json_severity = "severity";
        const QString                               QonarSonar::m_json_status = "status";
        const QString                               QonarSonar::m_json_author = "author";
        const QString                               QonarSonar::m_json_message = "message";
        const QString                               QonarSonar::m_json_component = "component";
        const QString                               QonarSonar::m_json_components = "components";
        const QString                               QonarSonar::m_json_line = "line";
        const QString                               QonarSonar::m_credential_file = ".config/qonar.crd";

        const QString                               QonarSonar::m_qonar_url_settings = "qonar_url";
        const QString                               QonarSonar::m_qonar_project_settings = "qonar_project";

        const QHash<QString, Constants::IssueType>  QonarSonar::m_sonarToQonar {
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

        QonarSonar::QonarSonar(QObject* parent):
            QObject(parent),
            p_nam(new QNetworkAccessManager(this)),
            m_credentials_ok(false)
        {
            connect(p_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
        }

        QMap<QString, QString>QonarSonar::projects()
        {
            readSettings();
            QUrl    url = m_url;
            QString path = url.path() + "/" + m_api_project;

            url.setPath(path);
            sendRequest(url);

            QJsonObject object = m_document.object();
            QMap<QString, QString> result;
            for(const QJsonValue& val : object[m_json_components].toArray())
            {
                QJsonObject project = val.toObject();
                result[project[m_json_project_key].toString()] = project[m_json_project_name].toString();
            }
            return result;
        }

        void QonarSonar::updateIssues(QList<QonarItem>& itemlist)
        {
            itemlist.clear();
            readSettings();
            if(m_url.isEmpty() || m_project.isEmpty())
            {
                if(!m_silent)
                {
                    QMessageBox::warning(Core::ICore::mainWindow(),
                                            tr( "No configuration"),
                                            tr( "Qonar is not configured for this project. "));
                }
                return;
            }

            QUrl url = m_url;
            QString path = url.path() + "/" + m_api_issues;


            url.setPath(path);
            QUrlQuery query(url.query());
            query.addQueryItem(m_api_issues_project_arg, m_project);
            query.addQueryItem(m_api_issues_resolved_arg, m_api_issues_resolved_value);
            url.setQuery(query);
            sendRequest(url);
            if(!m_error)
            {
                QJsonObject object = m_document.object();
                QJsonArray  jsonIssues = object[m_json_issues].toArray();
                for(const QJsonValue& val : jsonIssues)
                {
                    QJsonObject issue = val.toObject();
                    QonarItem   item;
                    item.issueType = m_sonarToQonar[issue[m_json_severity].toString()];
                    item.status = issue[m_json_status].toString();
                    item.user = issue[m_json_author].toString();
                    item.text = issue[m_json_message].toString();
                    QStringList file = issue[m_json_component].toString().split(":");
                    item.file = Utils::FilePath::fromString(m_projectDir + QDir::separator() + file.last());
                    item.line = issue[m_json_line].toInt();
                    itemlist.append(item);
                }
            }
        } // updateIssues

        void QonarSonar::setSilent(bool silent)
        {
            m_silent = silent;
        }

        void QonarSonar::sendRequest(QUrl& url)
        {
            qDebug() << "GET" << url;
            m_last_url = url;
            m_reply_finished = false;
            m_error = false;

            m_document = QJsonDocument();

            if(!m_credentials_ok) {
                m_credentials_ok = readCredentials();
                if (!m_credentials_ok) {
                    bool ok = false;
                    m_organization = QInputDialog::getText(Core::ICore::mainWindow(),
                                                       tr("Organization"),
                                                       tr("Organization"),
                                                       QLineEdit::Normal,
                                                       tr("Please enter your organization name"),
                                                       &ok);
                    if(ok)
                    {
                        m_token = QInputDialog::getText(Core::ICore::mainWindow(),
                                                           tr("Token"),
                                                           tr("Token"),
                                                           QLineEdit::Normal,
                                                           tr("Please enter a security token"),
                                                           &ok);
                        if(ok)
                        {
                            writeCredentials();
                            m_credentials_ok = true;
                        }
                    }
                }
            }

            if (m_credentials_ok) {
                QString qStr = url.query();
                if (!qStr.isEmpty()) qStr += "&organization=" + m_organization;
                else qStr += "organization=" + m_organization;
                url.setQuery(qStr);
            }

            QNetworkRequest networkRequest(url);
            //force json format response
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

            if(m_credentials_ok)
            {
                QString concatenated = m_token + ":";
                QByteArray data = concatenated.toLocal8Bit().toBase64();
                QString headerData = "Basic ";
                headerData = headerData + data;
                networkRequest.setRawHeader("Authorization", headerData.toLocal8Bit());
            }

            p_nam->get(networkRequest);

            while(!m_reply_finished) qApp->processEvents();
        }

        void QonarSonar::requestFinished(QNetworkReply* reply)
        {
            int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if(reply->error() == QNetworkReply::NoError)
            {
                if((status == 301) || (status == 302))
                {
                    QUrl redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
                    sendRequest(redirection);
                } else {
                    // Read the result
                    m_document = QJsonDocument::fromJson(reply->readAll());
                    m_reply_finished = true;
                    m_error = false;
                }
            } else {
                // TODO error handling
                if(status == 401 || status == 403)
                {
                    m_credentials_ok = false;
                    bool ok = false;
                    m_organization = QInputDialog::getText(Core::ICore::mainWindow(),
                                                       tr("Organization"),
                                                       tr("Organization"),
                                                       QLineEdit::Normal,
                                                       tr("Please enter your organization name"),
                                                       &ok);
                    if(ok)
                    {
                        m_token = QInputDialog::getText(Core::ICore::mainWindow(),
                                                           tr("Token"),
                                                           tr("Token"),
                                                           QLineEdit::Normal,
                                                           tr("Please enter a security token"),
                                                           &ok);
                        if(ok)
                        {
                            m_credentials_ok = true;
                            writeCredentials();
                            sendRequest(m_last_url);
                        }
                    }
                } else {
                    qWarning() << "ERROR" << reply->error();
                    m_reply_finished = true;
                    m_error = true;
                    QMessageBox::critical(Core::ICore::mainWindow(),
                                          tr("Error Occured"),
                                          tr("The following error occured (status: %1): \n").arg(status) + reply->errorString());
                }
            }
        }

        void QonarSonar::readSettings()
        {
            ProjectExplorer::ProjectTree* tree = ProjectExplorer::ProjectTree::instance();
            if(!tree) return;

            ProjectExplorer::Project* current_project = tree->currentProject();
            if(!current_project) return;

            m_projectDir = current_project->projectDirectory().toString();

            m_url = current_project->namedSettings(m_qonar_url_settings).toString();
            m_project = current_project->namedSettings(m_qonar_project_settings).toString();
        } // readSettings

        bool QonarSonar::readCredentials()
        {
            QString credential_file = qgetenv("HOME") + QDir::separator() + m_credential_file;
            QFileInfo file_info(credential_file);
            if (file_info.exists() && file_info.isFile()) {
                QFile file(credential_file);
                if(file.open(QIODevice::ReadOnly)) {
                    QTextStream stream(&file);
                    m_organization = stream.readLine();
                    m_token = stream.readLine();
                    file.close();
                    return true;
                }
                file.close();
                return false;
            }
            else {
                return false;
            }
        }

        void QonarSonar::writeCredentials()
        {
            QString credential_file = qgetenv("HOME") + QDir::separator() + m_credential_file;
            QFile file(credential_file);
            if(file.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&file);
                stream << m_organization << '\n' << m_token << '\n';
                file.close();
            }
        }
    } // namespace Internal
} // namespace Qonar
