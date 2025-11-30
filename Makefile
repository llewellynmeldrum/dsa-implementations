.PHONY: all clean run debug help h ? docs

# my personal makefile, to do the following:
# 1. Compile all $(SRC_EXT) files with selected compiler, into separate object files, in $(BUILD_DIR)
# 2. Link all obj files we've just build against eachother
# 3. include any libs through ldlibs, prefer using $(shell pkg-config <name>) pattern
# 4. Offer options to debug segfaults, using adsan, usan, etc
# Do all of the above, but also make it pretty with fancy tput headers lol
all: $(EXE)

EXE_DIR	:=bin
EXE_NAME :=test
EXE :=$(EXE_DIR)/$(EXE_NAME)


SRC_DIR	:=src
OBJ_DIR :=build

SRC_EXT	:=.cpp
OBJ_EXT :=.o


# compiler: goto 1-1 for info on excluding files
CXX	:=g++
SRC 	:=$(wildcard $(SRC_DIR)/*$(SRC_EXT)) 
OBJS 	:= $(patsubst $(SRC_DIR)/%$(SRC_EXT),$(OBJ_DIR)/%$(OBJ_EXT),$(SRC))


# flags for the compiler only 
CXXFLAGS	:=-Iinclude $(shell pkg-config --cflags fmt)
LDFLAGS		:=$(shell pkg-config --libs fmt)
LDLIBS		:=
ALLFLAGS	:=-std=c++20 -O0

ASAN_OPTIONS	:= 


CXX+=$(ALLFLAGS)


$(OBJ_DIR)/%$(OBJ_EXT) : $(SRC_DIR)/%$(SRC_EXT)
	@$(ECHO_COMP_BANNER)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# build executable (linking obj -> bin)
$(EXE): $(OBJ_DIR) $(OBJS)
	@$(ECHO_LINK_BANNER)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(EXE) $(LDLIBS)

# executing binary
run: $(EXE)
	@$(ECHO_EXE_BANNER)
	ASAN_OPTIONS=$(ASAN_OPTIONS) ./$(EXE) $(ARGS)

leaks: CXXFLAGS  += -g 
leaks: LDFLAGS +=  -g
leaks: $(EXE)
	@$(ECHO_EXE_BANNER)
	echo "Testing for leaks..."
	tput clear;
	MallocStackLogging=1 MallocStackLoggingNoCompact=1 leaks --atExit -- ./$(EXE) $(ARGS)
	
	
#NOTE: AVOID PUTTING TARGETS IN CLEAN COMMAND! BE VERY SPECIFIC SO YOU DONT NUKE YOUR SOURCE FILES ON ACCIDENT
clean: 
	$(ECHO_CLEAN_BANNER)
	rm -f build/*.o src/*.o bin/*

# ------------ DEBUGGING ------------ #
debug: $(EXE)
	lldb -o run -- $(EXE) $(TERM_COLS) $(ARGS)

	
# Address san: lower overhead than thread-san, cleaner stack traces,
asan: CXXFLAGS  += -fsanitize=address -fno-omit-frame-pointer -g
asan: LDFLAGS += -fsanitize=address -g
asan: LDLIBS  += -fsanitize=address
asan: ASAN_OPTIONS += abort_on_error=1
asan: clean run 

usan: CXXFLAGS  += -fsanitize=undefined -fno-omit-frame-pointer 
usan: LDFLAGS += -fsanitize=undefined 
usan: LDLIBS  += -fsanitize=undefined 
usan: clean run 


ausan: CXXFLAGS  += -g -fsanitize=address,undefined -fno-omit-frame-pointer
ausan: LDFLAGS += -fsanitize=address,undefined -g
ausan: CXXFLAGS+= -fsanitize-address-use-after-scope
ausan: clean run

tsan: CXXFLAGS  += -fsanitize=thread -fno-omit-frame-pointer 
tsan: LDFLAGS += -fsanitize=thread
tsan: LDLIBS  += -fsanitize=thread
tsan: clean run 

lsan: CXXFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer 
lsan: LDFLAGS += -fsanitize=address,undefined
lsan: ASAN_OPTIONS+=detect_leaks=1
lsan: clean run 

# ------------ EXTRAS ------------ #
$(OBJ_DIR):
	@mkdir -p $@

$(EXE_DIR):
	@mkdir -p $@

help h ? docs: HELP
.PHONY: HELP
HELP:
	
	@$(FMT_REV)
	@printf "===== MAKE VARIABLES =====\n"; $(FMT_RESET)
	@$(FMT_ALT1) 	printf "SRC      = $(SRC)\n" 
	@$(FMT_ALT2) 	printf "EXE_DIR  = $(EXE_DIR )\n"
	@$(FMT_ALT1)	printf "EXE_NAME = $(EXE_NAME)\n"
	@$(FMT_ALT2)	printf "EXE      = $(EXE)\n"
	@$(FMT_ALT1)	printf "SRC_DIR  = $(SRC_DIR)\n"
	@$(FMT_ALT2)	printf "OBJ_DIR  = $(OBJ_DIR)\n"
	@$(FMT_ALT1)	printf "SRC_EXT  = $(SRC_EXT)\n"
	@$(FMT_ALT2)	printf "OBJ_EXT  = $(OBJ_EXT)\n"
	@$(FMT_ALT1)	printf "CXX     = $(CXX)\n"
	@$(FMT_ALT2)	printf "SRC      = $(SRC)\n"
	@$(FMT_ALT1)	printf "OBJS     = $(OBJS)\n"
	@$(FMT_ALT2)	printf "CXXFLAGS  = $(CXXFLAGS)\n"
	@$(FMT_ALT1)	printf "LDFLAGS  = $(LDFLAGS)\n"
	@$(FMT_ALT2)	printf "LDLIBS   = $(LDLIBS)\n"
	@$(FMT_ALT1)	printf "ALLFLAGS = $(ALLFLAGS)\n"




# ------------ PRETTY PRINTING ------------ #
FMT_RESET	:=tput sgr0;
FMT_REDBANNER	:=tput rev; tput bold; tput setaf 1;
FMT_GREENBANNER	:=tput rev; tput bold; tput setaf 2;
FMT_YELLOWBANNER:=tput rev; tput bold; tput setaf 3;
FMT_REV		:=tput rev; tput bold;


FMT_ALT1	:= tput setaf 7;
FMT_ALT2	:= tput sgr0;


ECHO_LINK_BANNER := @$(FMT_YELLOWBANNER) 	printf " LINKING OBJ to BIN -> "; $(FMT_RESET) 	printf "\t"
ECHO_COMP_BANNER := @$(FMT_GREENBANNER)		printf " COMPILE SRC to OBJ -> "; $(FMT_RESET)	printf "\t"
ECHO_EXE_BANNER := @$(FMT_REDBANNER) 		printf " EXECUTING BINARY -> "; $(FMT_RESET)	printf "\t"
ECHO_CLEAN_BANNER:= @$(FMT_REV) 		printf " REMOVING EXEUTABLES AND OBJECT FILES... "; $(FMT_RESET)printf "\t"


# EXTRA MAKEFILE STUFF:
# :_______1-1_______: To exclude certain files from compilation:
# EXCLUDE_PAT	:= unit_tests.c
# SRC 		:= $(filter-out src/$(EXCLUDE_SRC), $(wildcard src/*.c))
