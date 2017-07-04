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
#include "QonarSonar.h"

namespace Qonar
{
    namespace Internal
    {
        QonarItemProvider::QonarItemProvider(QonarSonar* qonarSonar):
            QObject(qonarSonar),
            ptr_qonarSonar(qonarSonar)
        {
            m_itemsModel = new QonarModel(this);
            m_itemsModel->setQonarItemsList(&m_itemList);
        }

        void QonarItemProvider::refresh(bool silent)
        {
            ptr_qonarSonar->setSilent(silent);
            ptr_qonarSonar->updateIssues(m_itemList);
            m_itemsModel->qonarItemsListUpdated();
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
    } // namespace Internal
} // namespace Qonar
