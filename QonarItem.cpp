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
#include "QonarItem.h"

namespace Qonar
{
    namespace Internal
    {
        QIcon QonarItem::icon()
        {
            return toIcon(issueType);
        }

        QIcon QonarItem::toIcon(Constants::IssueType issueType)
        {
            switch(issueType)
            {
            case Constants::Info:
            {
                const static QIcon icon(":/icons/info");
                return icon;
            }
            case Constants::Minor:
            {
                const static QIcon icon(":/icons/minor");
                return icon;
            }
            case Constants::Major:
            {
                const static QIcon icon(":/icons/major");
                return icon;
            }
            case Constants::Critical:
            {
                const static QIcon icon(":/icons/critical");
                return icon;
            }

            case Constants::Blocker:
            {
                const static QIcon icon(":/icons/blocker");
                return icon;
            }
            } // switch
            return QIcon();
        } // icon
    } // namespace Internal
} // namespace Qonar
