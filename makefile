OBJS      := main.o lexer.o token.o util.o run.o variable.o compile.o

SRCPATH   := ./src/
TESTPATH  := ./test/

PROGRAM   := oto
MAKE      := make.exe -r
DEL       := del
MAKEOBJ   := gcc -c
CC        := gcc
CFLAGS    := -o $(PROGRAM) -O2 -Wall

TESTFLAGS := -O2 -Wall

all : $(PROGRAM)

%.o : $(SRCPATH)%.c makefile
	$(MAKEOBJ) $(SRCPATH)$*.c

$(PROGRAM) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

test_% : $(OBJS)
	$(MAKEOBJ) $(TESTPATH)test_$*.c
	$(CC) $(TESTFLAGS) -o test_$* $(filter-out main.o, $(OBJS)) test_$*.o
	test_$*.exe

clean : 
	-$(DEL) *.o
	-$(DEL) *.exe

run : $(PROGRAM).exe
	$(PROGRAM)