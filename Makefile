PROJECTNAME=tester
MICROCONTROLLER=atmega328
OPTIONS=-Os -DF_CPU=16000000 -std=c99 -Wall -mmcu=$(MICROCONTROLLER) -Wextra

all: build size upload

build:
	@avr-gcc $(OPTIONS) -c *.c
	@avr-gcc $(OPTIONS) -o $(PROJECTNAME).elf *.o
	@avr-objcopy -O ihex --change-section-lma .eeprom=0 $(PROJECTNAME).elf $(PROJECTNAME).hex

upload:
	@avrdude -v -p$(MICROCONTROLLER) -cusbasp -Uflash:w:$(PROJECTNAME).hex:i
	
clean:
	@rm *.elf *.hex *.o
	
size:
	@echo
	@avr-size -C --mcu=${MICROCONTROLLER} $(PROJECTNAME).elf
	
setfuses:
	@avrdude -cusbasp -p$(MICROCONTROLLER) -U lfuse:w:0xCE:m
	@avrdude -cusbasp -p$(MICROCONTROLLER) -U hfuse:w:0xD9:m
	@avrdude -cusbasp -p$(MICROCONTROLLER) -U efuse:w:0xFC:m
