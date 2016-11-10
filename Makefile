##############################################################################
HOME     = /Users/mdekauwe
CFLAGS   =  -g -Wall#-O3 #-g -Wall #-O3 #-g -Wall
ARCH     =  x86_64
INCLS    = -I./include
LIBS     = -L/opt/local/lib -lm -lnetcdf
CC       =  gcc
PROGRAM  =  get_lt_aussie_tmax
SOURCES  =  $(PROGRAM).c
OBJECTS = $(SOURCES:.c=.o)
RM       =  rm -f
##############################################################################

# top level create the program...
all: 		$(PROGRAM)

# Compile the src file...
$(OBJECTS):	$(SOURCES)
		$(CC) ${INCLS} $(CFLAGS) -c $(SOURCES)

# Linking the program...
$(PROGRAM):	$(OBJECTS)
		$(CC) $(OBJECTS) $(LIBS) ${INCLS} $(CFLAGS) -o $(PROGRAM)
		$(RM) $(OBJECTS)
# clean up...
clean:
		$(RM) $(OBJECTS) $(PROGRAM)

install:
		cp $(PROGRAM) $(HOME)/bin/$(ARCH)/.
		$(RM) $(OBJECTS)
##############################################################################
