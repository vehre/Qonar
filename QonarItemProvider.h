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
        class QonarSonar;
        class QonarItemProvider:public QObject
        {
            Q_OBJECT

            public:
                explicit QonarItemProvider(QonarSonar* qonarSonar);

                QonarModel* getItemsModel() const;

            signals:
            public slots:
                void refreshSilent();

                void refreshVerbose();

            private:
                void refresh(bool silent = false);

                QonarSonar*     ptr_qonarSonar;
                QList<QonarItem>m_itemList;
                QonarModel*     m_itemsModel;
        };
    } // namespace Internal
} // namespace Qonar

#endif // QONARITEMPROVIDER_H
