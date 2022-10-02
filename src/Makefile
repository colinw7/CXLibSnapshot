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
-std=c++17 \
-I$(INC_DIR) \
-I. \
-I../../CXLib/include \
-I../../CImageLib/include \
-I../../CFont/include \
-I../../CFileUtil/include \
-I../../CFile/include \
-I../../COS/include \
-I../../CStrUtil/include \
-I../../CMath/include \
-I../../CUtil/include \
-I../../CTimer/include \

LFLAGS = \
$(LEBUG) \
-L$(LIB_DIR) \
-L../../CRenderer/lib \
-L../../CXLib/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CArgs/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CConfig/lib \
-L../../CUtil/lib \
-L../../CRegExp/lib \
-L../../CTimer/lib \

LIBS = \
-lCXLib -lCXRenderer -lCRenderer -lCConfig -lCImageLib -lCFont -lCTimer -lCArgs \
-lCFileUtil -lCConfig -lCFile -lCMath -lCUtil -lCOS -lCRegExp -lCStrUtil \
-lXt -lX11 -lpng -ljpeg -ltre -lfreetype

.SUFFIXES: .cpp

$(CPP_OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CPP_CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CXLibSnapshot: $(CPP_OBJS)
	$(CPP_CC) -o $(BIN_DIR)/CXLibSnapshot $(CPP_OBJS) $(LFLAGS) $(LIBS)
