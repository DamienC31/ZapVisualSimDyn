bin_PROGRAMS = fr


fr_SOURCES = \
main.cpp \
cell.cpp \
structure.cpp \
vertex2D.cpp \
vertex3Dp.cpp \
vertex3D.cpp \
world.cpp \
UInterface.cpp \
microsim.cpp \
powersim.cpp \
misc.cpp \
face.cpp \
parameter.cpp \
button.cpp \
buttonbar.cpp \
texture.cpp \
residsim.cpp \
render.cpp \
maths.cpp \
user.cpp \
tga.cpp \
console.cpp \
conpos.cpp


LDADD= \
  -lglut            \
  @X_LIBS@      \
  @GL_LIBS@     \
  -lm 

LDFLAGS=-pg

INCLUDES = \
  $(X_CFLAGS) -O2 -pg
