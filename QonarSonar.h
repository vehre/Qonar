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
#ifndef QONARSONAR_H
#define QONARSONAR_H

#include "QonarItem.h"
#include <QJsonDocument>
#include <QObject>
#include <QUrl>

class QNetworkReply;
class QNetworkAccessManager;
#include <QPair>
namespace Qonar
{
    namespace Internal
    {
        class QonarSonar:public QObject
        {
            Q_OBJECT

            public:
                explicit QonarSonar(QObject* parent = 0);

                QMap<QString, QString> projects();

                void updateIssues(QList<QonarItem>& itemlist);

                void setSilent(bool silent);

                void setUrl(const QUrl& url);

                void setUrl(const QString& url);

            signals:
            private slots:
                void sendRequest(QUrl& url);

                void requestFinished(QNetworkReply* reply);

            private:
                void readSettings();
                QJsonDocument           m_document;
                bool                    m_silent;
                QNetworkAccessManager*  p_nam;
                QUrl                    m_url;
                QUrl                    m_last_url;
                bool                    m_reply_finished;
                bool                    m_error;
                QString                 m_project;
                QString                 m_projectDir;
                bool                    m_credentials_ok;
                QString                 m_username;
                QString                 m_password;

                static const QString                                m_format_arg;
                static const QString                                m_format_value;
                static const QString                                m_api_issues;
                static const QString                                m_api_issues_project_arg;
                static const QString                                m_api_issues_resolved_arg;
                static const QString                                m_api_issues_resolved_value;
                static const QString                                m_api_project;
                static const QString                                m_json_project_name;
                static const QString                                m_json_project_key;
                static const QString                                m_json_issues;
                static const QString                                m_json_severity;
                static const QString                                m_json_status;
                static const QString                                m_json_author;
                static const QString                                m_json_message;
                static const QString                                m_json_component;
                static const QString                                m_json_components;
                static const QString                                m_json_line;
                static const QHash<QString, Constants::IssueType>   m_sonarToQonar;
            public:
                static const QString                                m_qonar_url_settings;
                static const QString                                m_qonar_project_settings;
        };
    } // namespace Internal
} // namespace Qonar

#endif // QONARSONAR_H
