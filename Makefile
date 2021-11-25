SHELL=/bin/bash
CXX			:=	g++

VERFILE		:=	version.txt

VER			:=	$(subst ., ,$(file <$(VERFILE)))

MAJOR		:=	$(word 1, $(VER))
MINOR		:=	$(word 2, $(VER))
PATCH		:=	$(word 3, $(VER))

#can be debug|release
BUILD_TYPE	:=	debug
VERSION		:=	v$(MAJOR).$(MINOR).$(PATCH)-$(BUILD_TYPE)

CXXARGS		:=	-DVERSION='"$(VERSION)"' -pthread -std=c++2a

ifeq ($(BUILD_TYPE),debug)
	CXXARGS	+=	-D_DEBUG -g -Og
else
	CXXARGS	+=	-Ofast
endif

EXCLUDE		:=	./Interface/%

CPPFILES	:=	$(filter-out $(EXCLUDE), $(shell find ./ -name '*.cpp'))
HFILES		:=	$(filter-out $(EXCLUDE), $(shell find ./ -name '*.h'))
THIS		:=	./Makefile

OBJFILES	:=	$(filter-out $(EXCLUDE), $(patsubst %.cpp, %.cpp.o, $(CPPFILES)))
FOUNDOFILES	:=	$(shell find ./ -name '*.o')

PROJECT		:=	Tea6


all: $(PROJECT)

$(PROJECT): $(OBJFILES)
	@echo Linking $@
	@$(CXX) -o $@ $? $(CXXARGS)

%.cpp.o: %.cpp $(HFILES) $(THIS) $(VERFILE)
	@echo Compiling $@
	@$(CXX) -o $@ $< -c $(CXXARGS) 

.PHONY:	clean patch minor major show-version tag

clean:
	@echo Cleaning files
	@rm -f $(FOUNDOFILES)
	@rm -f $(PROJECT)

patch:
	$(file >$(VERFILE),$(MAJOR).$(MINOR).$(shell echo $$(( $(PATCH) + 1 ))))
	@echo New patch: $(file <$(VERFILE))

minor:
	$(file >$(VERFILE),$(MAJOR).$(shell echo $$(( $(MINOR) + 1 ))).0)
	@echo New minor: $(file <$(VERFILE))

major:
	$(file >$(VERFILE),$(shell echo $$(( $(MAJOR) + 1 ))).0.0)
	@echo New major: $(file <$(VERFILE))

show-version:
	@echo Current version: $(VERSION)

tag:
	@git tag -a $(VERSION) -m "new version"
	@git push --tags origin
	@echo Tag $(VERSION) added