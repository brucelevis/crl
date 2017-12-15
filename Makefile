#set project directories
DEBUG    ?= 0
PROJDIR  := ./
SDIRS	  = src src/systems
BUILDDIR := $(PROJDIR)build
IDIRS 	 := -I$(PROJDIR)include
LDIRS 	 := -L$(PROJDIR)lib/Windows64 -L$(PROJDIR)/lib/Linux64

#set compiler
CC=g++

#set flags
CFLAGS=$(IDIRS) -std=c++11 -Wall -Werror
OSFLAGS=

#set libs
LIBS=-lBearLibTerminal

#set extension
CPP_EXT := cpp

#name of the final executable
TARGET = crl

#change some os specific settings when needed
ifeq ($(OS),Windows_NT)
	OSFLAGS=-mwindows
    RM = del /F /Q 
    RMDIR = -RMDIR /S /Q
    MKDIR = mkdir
    ERRIGNORE = 2>nul
	SEP=\\
else
	LIBS += -lfreetype2 -lpicopng -lGL -lX11 -ldl
    RM = rm -rf 
    RMDIR = rm -rf 
    MKDIR = mkdir -p
    ERRIGNORE = 2>/dev/null
	SEP=/
endif

#remove spaces
PSEP = $(strip $(SEP))

#create list of source directories
SOURCEDIRS = $(foreach dir, $(SDIRS), $(addprefix $(PROJDIR), $(dir)))

#determine if to build debug or release
ifeq ($(DEBUG),1)
	ABUILDDIR := $(PROJDIR)$(PSEP)build$(PSEP)debug
	CFLAGS += -DDEBUG -g	
	CFLAGS += $(OSFLAGS)
else
	ABUILDDIR := $(PROJDIR)$(PSEP)build$(PSEP)release
	CFLAGS += -O2 -DNDEBUG
	CFLAGS += $(OSFLAGS)
endif

#set target directories
TARGETDIRS = $(foreach dir, $(SDIRS), $(addprefix $(ABUILDDIR)/, $(dir)))

#add sources and object locations
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.$(CPP_EXT))) 
OBJS = $(subst ./,$(ABUILDDIR)/,$(SOURCES:.$(CPP_EXT)=.o))

#add this list to VPATH, the place make will look for the source files
VPATH = $(SOURCEDIRS)

# Hide or not the calls depending of VERBOSE
ifeq ($(VERBOSE),TRUE)
    HIDE =  
else
    HIDE = @
endif

#define the function that will generate each rule
define generateRules
$(1)/%.o: %.$(CPP_EXT)
	@echo Building $$(subst /,$$(PSEP),$$@)
	$(HIDE) $(CC) -c -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) $(CFLAGS)
endef

.PHONY: all clean debug install directories

all: directories $(TARGET)
$(TARGET): directories

#link target
$(TARGET): $(OBJS)
	@echo Linking
	$(CC) $(OBJS) -o $(ABUILDDIR)$(PSEP)$(TARGET) $(LDIRS) $(LIBS) $(CFLAGS)

$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories: 
	@echo Creating directories
ifeq ($(OS),Windows_NT)
	$(HIDE) if not exist $(subst /,$(PSEP),$(TARGETDIRS)) $(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
else
	$(HIDE) $(MKDIR) $(subst /,$(PSEP),$(TARGETDIRS)) $(ERRIGNORE)
endif
	
clean:
	$(RMDIR) $(subst /,$(PSEP),$(BUILDDIR)) $(ERRIGNORE)
	$(RM) $(TARGET) $(ERRIGNORE)

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

install: crl
	install -d $(PREFIX)/lib
	install -m 644 $(LDIR)/*.so $(PREFIX)/lib
