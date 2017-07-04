Qonar
=======
Sonar plugin for QtCreator

This plugin connects QtCreator with a SonarQube server. 
It is widely inspired by the TODO plugin. 

### build
To build this plugin (like every QtCreator plugin) you need to have QtCreator source tree and build tree available.

    mkdir Qonar-build
    cd Qonar-build
    export QTC_SOURCE=PATH_TO_QtCreator_SOURCES
    export QTC_BUILD=PATH_TO_QtCreator_BUILD_DIR
    qmake -r PATH/TO/SOURCES/qonar.pro
    make
    
While building, it automatically installs itself in the user directory for QtCreator to find it :

    $HOME/.local/share/data/QtProject/qtcreator/plugins/QTC_VERSION/

Do not forget to activate the plugin in QtCreator :

    Help => About Plugins => Check the plugin

### TODO
    * Add authentication. => Implemented, but not tested yet. 
    * Page management (Sonar limits to 500 issues max)
    * Add in app resolution (false positive etc.)
    
