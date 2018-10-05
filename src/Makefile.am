AUTOMAKE_OPTIONS = foreign
SUBDIRS = 

USE_BEARLIBTERMINAL_FLAG = $(if $(USE_BEARLIBTERMINAL), -DUSE_BEARLIBTERMINAL, )
INCLUDES_BEARLIBTERMINAL = $(if $(USE_BEARLIBTERMINAL), -L$(top_srcdir)/lib/Linux64 -lBearLibTerminal, )

USE_NCURSES_FLAG = $(if $(USE_NCURSES), -DUSE_NCURSES, )
INCLUDES_NCURSES = $(if $(USE_NCURSES), -lncurses, )

CPPFLAGS = $(USE_BEARLIBTERMINAL_FLAG) $(USE_NCURSES_FLAG) -Wall -Werror -std=c++11 -O2 -I$(top_srcdir)/include -I$(top_srcdir)/lib/include
LDFLAGS = -lncurses $(INCLUDES_NCURSES) $(INCLUDES_BEARLIBTERMINAL)

bin_PROGRAMS = crl
crl_SOURCES = app.cpp components.cpp console/bearlibterminalconsole.cpp console/iconsole.cpp ecs.cpp generators/historygenerator.cpp generators/mapgenerator.cpp logger.cpp main.cpp map.cpp randomgenerator.cpp systems/aisystem.cpp systems/attacksystem.cpp systems/camerasystem.cpp systems/containersystem.cpp systems/destructiblesystem.cpp systems/inputsystem.cpp systems/mapsystem.cpp systems/movementsystem.cpp systems/rendersystem.cpp systems/uisystem.cpp tiles.cpp ../include/app.h ../include/BearLibTerminal.h ../include/components.h ../include/console/bearlibterminalconsole.h ../include/console/iconsole.h ../include/deprecated.h ../include/ecs.h ../include/generators/historygenerator.h ../include/generators/mapgenerator.h ../include/logger.h ../include/map.h ../include/randomgenerator.h ../include/systems/aisystem.h ../include/systems/attacksystem.h ../include/systems/camerasystem.h ../include/systems/containersystem.h ../include/systems/destructiblesystem.h ../include/systems/inputsystem.h ../include/systems/isystem.h ../include/systems/mapsystem.h ../include/systems/movementsystem.h ../include/systems/rendersystem.h ../include/systems/systemmessages.h ../include/systems/systems.h ../include/systems/uisystem.h ../include/tiles.h ../include/console/ncursesconsole.h console/ncursesconsole.cpp

