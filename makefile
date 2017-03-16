CC      = cc
CFLAGS  = -O -Wall
LD      = cc
CP      = cp
MV      = mv
RM      = rm
E       =
O       = .o

OBJ     = raw$(O) command$(O) display$(O) gap$(O) key$(O) search$(O) main$(O)

zepto$(E) : $(OBJ)
	$(LD) -o zepto$(E) $(OBJ)

raw$(O): raw.c header.h raw.h
	$(CC) $(CFLAGS) -c raw.c

command$(O): command.c header.h raw.h
	$(CC) $(CFLAGS) -c command.c

display$(O): display.c header.h raw.h
	$(CC) $(CFLAGS) -c display.c

gap$(O): gap.c header.h raw.h
	$(CC) $(CFLAGS) -c gap.c

key$(O): key.c header.h raw.h
	$(CC) $(CFLAGS) -c key.c

search$(O): search.c header.h raw.h
	$(CC) $(CFLAGS) -c search.c

main$(O): main.c header.h raw.h
	$(CC) $(CFLAGS) -c main.c

clean:
	-$(RM) $(OBJ) zepto$(E)

install:
	-$(CP) zepto$(E) $(HOME)/bin/zep
	-$(MV) zepto$(E) $(HOME)/bin/zepto
