TEMPLATE = lib
TARGET = $$qtLibraryTarget(exampleshareplugin)
CONFIG += plugin
DEPENDPATH += .

CONFIG += link_pkgconfig
PKGCONFIG += nemotransferengine-qt5

# Input
HEADERS += \
    exampleplugininfo.h \
    exampleuploader.h \
    exampleshareplugin.h

SOURCES += \
    exampleplugininfo.cpp \
    exampleuploader.cpp \
    exampleshareplugin.cpp

OTHER_FILES += \
    ExampleShareUI.qml

shareui.files = *.qml
shareui.path = /usr/share/nemo-transferengine/plugins

target.path = $$LIBDIR/nemo-transferengine/plugins
INSTALLS += target shareui

# NOTE: the translations here assume that generic sharing UI system will pick up the
# translations from specific /usr/share/translations/nemotransferengine/ with
# specific name where '-' separates locale code at the end.
# It might work for time being, but it's preferred if plugins take the responsibility
# for loading translations in sharing plugin if that uses translations, and/or from
# qml code. Latter commonly by importing an own module where c++ side instantiates
# and installs QTranslator(s) to the qApp on initializeEngine().

TS_FILE = $$OUT_PWD/example_share_plugin.ts
EE_QM = $$OUT_PWD/example_share_plugin_eng_en.qm

ts.commands += lupdate . -ts $$TS_FILE
ts.CONFIG += no_check_exist no_link
ts.output = $$TS_FILE
ts.input = ..

ts_install.files = $$TS_FILE
ts_install.path = /usr/share/translations/source
ts_install.CONFIG += no_check_exist

# should add -markuntranslated "-" when proper translations are in place (or for testing)
engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist no_link
engineering_english.depends = ts
engineering_english.input = $$TS_FILE
engineering_english.output = $$EE_QM

engineering_english_install.path = /usr/share/translations/nemotransferengine
engineering_english_install.files = $$EE_QM
engineering_english_install.CONFIG += no_check_exist

TS_FI_FILE = $$PWD/translations/example_share_plugin-fi.ts
QM_FI_FILE = example_share_plugin-fi.qm

finnish.commands += lrelease -idbased $$TS_FI_FILE -qm $$QM_FI_FILE
finnish.CONFIG += no_check_exist no_link
finnish.depends = ts
finnish.input = $$TS_FI_FILE
finnish.output = $$QM_FI_FILE

finnish_install.path = /usr/share/translations/nemotransferengine
finnish_install.files = $$QM_FI_FILE
finnish_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += ts engineering_english finnish

PRE_TARGETDEPS += ts engineering_english finnish

INSTALLS += ts_install engineering_english_install finnish_install

OTHER_FILES += \
    rpm/* \
    translations/*
