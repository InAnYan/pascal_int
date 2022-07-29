EXEC_NAME=pascal_inter2.out

CC=clang++
LD=clang++

BIN_DIR=bin
INC_DIR=include
SRC_DIR=src
OBJ_DIR=obj
SUBDIRS=.

INCLUDES_DIRS=
LIBS_DIRS=/usr/local/lib

LIBS=
DEFINES=

CFLAGS=-g -Wall -Wno-non-c-typedef-for-linkage
LDFLAGS=

DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJ_DIR)/$*.d

FULL_EXEC=$(BIN_DIR)/$(EXEC_NAME)
FULL_CFLAGS=-c $(CFLAGS) -I$(INC_DIR) $(addprefix -I, $(INCLUDES_DIRS)) $(addprefix -D, $(DEFINES)) $(DEPFLAGS)
FULL_LDFLAGS=$(LDFLAGS) $(addprefix -L, $(LIBS_DIRS)) $(addprefix -l, $(LIBS))

SRC_SUBDIRS=$(addprefix $(SRC_DIR)/, $(SUBDIRS))
INC_SUBDIRS=$(addprefix $(INC_DIR)/, $(SUBDIRS))

INCS=$(wildcard *.hpp $(foreach fd, $(INC_SUBDIRS), $(fd)/*.hpp))
SRCS=$(wildcard *.cpp $(foreach fd, $(SRC_SUBDIRS), $(fd)/*.cpp))

OBJS=$(subst $(SRC_DIR), $(OBJ_DIR), $(SRCS:.cpp=.o))
DEPFILES := $(OBJS:.o=.d)

all: $(FULL_EXEC)

$(FULL_EXEC): $(OBJS)
	$(CC) $(FULL_LDFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(FULL_CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*.exe $(BIN_DIR)/*.out $(BIN_DIR)/*.bin $(OBJ_DIR)/*

$(DEPFILES):

include $(wildcard $(DEPFILES))
