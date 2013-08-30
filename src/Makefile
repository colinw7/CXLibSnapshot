C_CC   = gcc
CPP_CC = g++

RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib
BIN_DIR = ../bin

all: $(BIN_DIR)/CXLibSnapshot

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CXLibSnapshot

CPP_SRC = \
CXLibSnapshot.cpp \

CPP_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(CPP_SRC))

CPPFLAGS = \
-I$(INC_DIR) \
-I. \
-I../../CXLib/include \
-I../../CImageLib/include \
-I../../CFont/include \
-I../../CFile/include \
-I../../COS/include \
-I../../CStrUtil/include \
-I../../CMath/include \
-I../../CUtil/include \
-I../../CTimer/include \

LFLAGS = \
$(LEBUG) \
-L$(LIB_DIR) \
-L../../CXLib/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CArgs/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CConfig/lib \
-L../../CTimer/lib \
-L../../CImageLib/lib \

LIBS = \
-lCXLib -lCConfig -lCImageLib -lCFont -lCTimer -lCArgs \
-lCFile -lCOS -lCStrUtil -lXt -lX11 -lpng -ljpeg

.SUFFIXES: .cpp

$(CPP_OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CPP_CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CXLibSnapshot: $(CPP_OBJS)
	$(CPP_CC) -o $(BIN_DIR)/CXLibSnapshot $(CPP_OBJS) $(LFLAGS) $(LIBS)
