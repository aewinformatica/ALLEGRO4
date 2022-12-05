CC     = gcc
WINDRES= windres
RM     = rm -f
OBJS   = main.o \
         mappyal.o \
         AppResource.res

LIBS   = -mwindows -lalpng -lalleg
CFLAGS =

.PHONY: Warbirds.exe clean clean-after

all: Warbirds.exe

clean:
	$(RM) $(OBJS) Warbirds.exe

clean-after:
	$(RM) $(OBJS)

Warbirds.exe: $(OBJS)
	$(CC)  -o $@ $(OBJS) $(LIBS)

main.o: main.c warbirds.h
	$(CC)  -c $< -o $@ $(CFLAGS)

mappyal.o: mappyal.c
	$(CC)  -c $< -o $@ $(CFLAGS)

AppResource.res: AppResource.rc
	$(WINDRES) -i AppResource.rc -J rc -o AppResource.res -O coff

