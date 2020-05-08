TEMPLATE = subdirs

SUBDIRS =\
    OsgExtern\
    SceneCore\
    Map\
    SpaceEnv\
    PlotManager\
	LoadMap\
	LoadQmlMap

OsgExtern.file = ./OsgExtern/OsgExtern_My.pro
SceneCore.file = ./SceneCore/SceneCore_My.pro
Map.file = ./Map/Map_My.pro
SpaceEnv.file = ./SpaceEnv/SpaceEnv_My.pro

CONFIG *= ordered
