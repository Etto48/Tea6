CXX			:=	g++

MAJOR		:=	0
MINOR		:=	0
PATCH		:=	1

#can be debug|release
BUILD_TYPE	:=	debug
VERSION		:=	v$(MAJOR).$(MINOR).$(PATCH)-$(BUILD_TYPE)

CXXARGS		:=	-DVERSION=\"$(VERSION)\" -pthread -std=c++2a

ifeq ($(BUILD_TYPE),debug)
	CXXARGS	+=	-D_DEBUG -g -Og
else
	CXXARGS	+=	-Ofast
endif

CPPFILES	:=	$(shell find ./ -name '*.cpp')
HFILES		:=	$(shell find ./ -name '*.h')
THIS		:=	./Makefile

OBJFILES	:=	$(patsubst %.cpp, %.cpp.o, $(CPPFILES))
FOUNDOFILES	:=	$(shell find ./ -name '*.o')

PROJECT		:=	Tea6


all: $(PROJECT)

$(PROJECT): $(OBJFILES)
	@echo Linking $@
	@$(CXX) -o $@ $(CXXARGS) $?

%.cpp.o: %.cpp $(HFILES) $(THIS)
	@echo Compiling $@
	@$(CXX) -o $@ -c $(CXXARGS) $<

.PHONY:	clean

clean:
	@echo Cleaning files
	@rm -f $(FOUNDOFILES)
	@rm -f $(PROJECT)
