CXX			:=	g++

MAJOR		:=	0
MINOR		:=	0
PATCH		:=	1
VERSION		:=	$(MAJOR).$(MINOR).$(PATCH)

CXXARGS		:=	-O3 -DVERSION=$(VERSION)

CPPFILES	:=	$(shell find ./ -name '*.cpp')
HFILES		:=	$(shell find ./ -name '*.h')

OBJFILES	:=	$(patsubst %.cpp, %.cpp.o, $(CPPFILES))
FOUNDOFILES	:=	$(shell find ./ -name '*.o')

PROJECT		:=	Tea6


all: $(PROJECT)

$(PROJECT): $(OBJFILES)
	@echo Linking $@
	@$(CXX) -o $@ $(CXXARGS) $?

%.cpp.o: %.cpp $(HFILES)
	@echo Compiling $@
	@$(CXX) -o $@ $(CXXARGS) $^

.PHONY:	clean

clean:
	@echo Cleaning all obj files
	@rm -f $(FOUNDOFILES)
