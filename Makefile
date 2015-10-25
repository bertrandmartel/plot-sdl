CXX         = gcc
STATIC      = libplotsdl.a
LIB_NAME    = plotsdl
EXEC        = plot
MAIN_PATH   = plot-sdl-test/main.c
CFLAGS      = -O3 -g -Werror
LDFLAGS     = -lm -lSDL2 -lSDL2_ttf
OBJECT_DIR  = bin
OBJECTS     = plot.o linked_list.o
VPATH       = plot-sdl-lib
HEADERS     = -I${VPATH}/plotsdl
RELEASE_DIR = release
INCLUDE_DIR = include
OBJS        = $(patsubst %,$(OBJECT_DIR)/%,$(OBJECTS))

default: init ${STATIC}
	$(CXX) -I${RELEASE_DIR}/${INCLUDE_DIR} -L${RELEASE_DIR} ${MAIN_PATH} $(CFLAGS) -o ${RELEASE_DIR}/${EXEC} -l${LIB_NAME} $(LDFLAGS)

init:
	mkdir -p ${OBJECT_DIR}

$(OBJECT_DIR)/%.o: %.c
	$(CXX) $(HEADERS) -c  $< $(CFLAGS) -o $@ $(LDFLAGS)

${STATIC}: $(OBJS)
	mkdir -p $(RELEASE_DIR)
	mkdir -p ${RELEASE_DIR}/${INCLUDE_DIR}
	cp -r ${VPATH}/plotsdl ${RELEASE_DIR}/${INCLUDE_DIR}/
	ar rcs ${RELEASE_DIR}/$@ $(OBJS)

clean:
	rm -fr ${OBJECT_DIR}/*.o

mrproper:
	rm -fr ${OBJECT_DIR}/*.o ${RELEASE_DIR}
