# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:wakedemo.elf buzzer.elf

#additional rules for files
wakedemo.elf: ${COMMON_OBJECTS} wakedemo.o wdt_handler.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

buzzer.elf: ${COMMON_OBJECTS} buzzer.o wakedemo.o ../lib/libTimer.a
	${CC} ${CFLAGS} -o $@ $^

load: wakedemo.elf buzzer.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
