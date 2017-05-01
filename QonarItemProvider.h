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
#ifndef QONARITEMPROVIDER_H
#define QONARITEMPROVIDER_H

#include "Constants.h"
#include "QonarItem.h"
#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;
namespace Qonar
{
    namespace Internal
    {
        class QonarModel;
        class QonarItemProvider:public QObject
        {
            Q_OBJECT

            public:
                explicit QonarItemProvider(QObject* parent = 0);

                void setUrl(const QString& url);

                void setUrl(const QUrl& url);

                QString server() const;

                void setServer(const QString& server);

                int server_port() const;

                void setServer_port(int server_port);

                QString server_path() const;

                void setServer_path(const QString& server_path);

                QString project() const;

                void setProject(const QString& project);

                QString scheme() const;

                void setScheme(const QString& scheme);

                QonarModel* getItemsModel() const;

            signals:
            public slots:
                void refreshSilent();
                void refreshVerbose();

                void refreshDone(QNetworkReply* reply);

            private:
                void refresh(bool silent=false);
                void getUrl(const QUrl& url);

                QString                                             m_server;
                QString                                             m_scheme;
                int                                                 m_server_port;
                QString                                             m_server_path;
                QString                                             m_project;
                QString                                             m_projectDir;
                QNetworkAccessManager*                              p_nam;
                QList<QonarItem>                                    m_itemList;
                QonarModel*                                         m_itemsModel;
                static const QString                                m_api_issue;
                static const QString                                m_project_arg;
                static const QString                                m_resolved_arg;
                static const QString                                m_resolved_value;
                static const QHash<QString, Constants::IssueType>   m_sonarToQonar;
        };
    } // namespace Internal
} // namespace Qonar

#endif // QONARITEMPROVIDER_H
