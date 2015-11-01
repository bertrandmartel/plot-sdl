CXX         = gcc
SHARED      = libplotsdl_wrapper.so
LIB_NAME    = plotsdl
EXEC        = plot
MAIN_PATH   = c/main.c
CFLAGS      = -fPIC -O3 -g -Werror
LDFLAGS     = -lplotsdl -lm -lSDL2 -lSDL2_ttf 
LIBS        = -L../release
OBJECT_DIR  = bin
OBJECTS     = wrapper.o plotsdl_interface.o
VPATH       = c
HEADERS     = -I../release/include -I${VPATH} -I/usr/lib/jvm/java-1.7.0-openjdk-i386/include
RELEASE_DIR = release
INCLUDE_DIR = include
OBJS        = $(patsubst %,$(OBJECT_DIR)/%,$(OBJECTS))

default: init ${SHARED}

init:
	mkdir -p ${OBJECT_DIR}

$(OBJECT_DIR)/%.o: %.c
	$(CXX) $(HEADERS) $(LIBS) -c  $< $(CFLAGS) -o $@ $(LDFLAGS)

${SHARED}: $(OBJS)
	$(CXX) -shared $(CFLAGS) $(LIBS) $(OBJS) -o $@ $(LIBS) $(LDFLAGS)

clean:
	rm -fr ${OBJECT_DIR}/*.o

mrproper:
	rm -fr ${OBJECT_DIR}/*.o ${RELEASE_DIR}
