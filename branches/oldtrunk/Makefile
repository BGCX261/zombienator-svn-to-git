###############################################################################
# Makefile for the project Zombienator
###############################################################################

## General Flags
PROJECT = zombienator
VIDEOMODE = 2

# Change if your programmer is different
AVRDUDE_PROGRAMMER = stk500v1
AVRDUDE_PORT = COM8	   # programmer connected to serial device

# Don't change unless you really know whats going on
MCU = atmega644
TARGET = $(PROJECT).elf
CC = avr-gcc


## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=28636360UL -Os -DVIDEO_MODE=$(VIDEOMODE) -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 
CFLAGS += -DINTRO_LOGO=1 -DMIXER_CHAN4_TYPE=0 -DSCREEN_SECTIONS_COUNT=2 -DINCLUDE_DEFAULT_WAVES=1 -DVRAM_TILES_V=36

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map

INCLUDES = -I..


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = uzeboxVideoEngineCore.o uzeboxCore.o uzeboxSoundEngine.o uzeboxSoundEngineCore.o uzeboxVideoEngine.o $(PROJECT).o structures.o videoengine.o mechanics.o necromancy.o guns.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

AVRDUDE = avrdude
AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss size

# Program the device.  
program: $(PROJECT).hex $(PROJECT).eep	
	$(AVRDUDE) $(AVRDUDE_FLAGS) -B 1 -U flash:w:$< 

## Compile
uzeboxVideoEngineCore.o: kernel/uzeboxVideoEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxSoundEngineCore.o: kernel/uzeboxSoundEngineCore.s
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uzeboxCore.o: kernel/uzeboxCore.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxSoundEngine.o: kernel/uzeboxSoundEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uzeboxVideoEngine.o: kernel/uzeboxVideoEngine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

$(PROJECT).o: $(PROJECT).c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<	

$structures.o: structures.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
	
$videoengine.o: videoengine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

$mechanics.o: mechanics.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
	
$necromancy.o: necromancy.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
	
$guns.o: guns.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<


##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(INCFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

