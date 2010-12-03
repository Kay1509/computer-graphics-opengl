#! smake
#---------------------------------------------------
#   Linux Makefile for Computer Graphics:   Assignment 5

# A. Hanson

#   B481/B581
# Indiana University
#  November 2000
#---------------------------------------------------
TARGET = ps5OFFtemplate.linux
SRCS = ps5OFFtemplate.c readoff.c

LN = ln -s
MV = mv
RM = -rm -rf

CC = cc

# for Gnu cc, add the following:
#CC = /usr/local/gnu/bin/gcc
#DGA = /usr/openwin/lib
#LD_LIBS = -L$(GLUT) -L$(DGA) -R$(DGA) -lglut -lGLU -lGL -lXi -lXmu -lXext -lX11 -lm
# GLUT = /l/glut/lib
#GLINC = /l/glut/include

LD_LIBS = -lglut -lGLU -lGL  -lm
CFLAGS =  -c

OBJS = $(SRCS:.c=.o)

.c.o:
	($(CC) $(CFLAGS)  $<)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)  $(LD_LIBS)

all     : $(TARGET) 

default : $(all)

clean:
	-$(RM) $(OBJS) $(TARGET) *~

depend:
	makedepend -f Makefile.linux $(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.


