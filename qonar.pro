DEFINES += QONAR_LIBRARY
QT += network
# Qonar files

SOURCES += qonarplugin.cpp \
    QonarOutputPane.cpp \
    QonarModel.cpp \
    QonarItem.cpp \
    QonarOutputTreeView.cpp \
    QonarItemProvider.cpp \
    QonarSettings.cpp \
    QonarSonar.cpp

HEADERS += qonarplugin.h \
        qonar_global.h \
    QonarOutputPane.h \
    QonarModel.h \
    QonarItem.h \
    Constants.h \
    QonarOutputTreeView.h \
    QonarItemProvider.h \
    QonarSettings.h \
    QonarSonar.h

OTHER_FILES *= uncrustify.cfg header

# Qt Creator linking

## Either set the IDE_SOURCE_TREE when running qmake,
## or set the QTC_SOURCE environment variable, to override the default setting
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = $$(QTC_SOURCE)
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = "/home/jpillon/Devel/QtCreator/qt-creator-opensource-src-4.2.2"

## Either set the IDE_BUILD_TREE when running qmake,
## or set the QTC_BUILD environment variable, to override the default setting
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = "/home/jpillon/Devel/QtCreator/qt-creator-opensource-src-4.2.2-build"

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = Qonar
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin projectexplorer

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)

RESOURCES += \
    qonar.qrc
QMAKE_CXXFLAGS *= -Wall -Werror

DISTFILES += \
    sonar-project.properties \
    README.md

FORMS += \
    QonarSettings.ui
