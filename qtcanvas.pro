include(buildInfo.pri)

TEMPLATE = subdirs

DESTDIR = $${BIN_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}

SUBDIRS += \
    qtCanvas \
    tests

