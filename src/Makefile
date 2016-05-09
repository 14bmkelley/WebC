TARGET    = server
CC        = gcc
CCFLAGS   = -std=c89 -pedantic -Wall -Werror
LDFLAGS   = -lm
BUILDPATH = ../build/
SOURCES   = $(wildcard *.c)
INCLUDES  = $(wildcard *.h)
OBJECTS   = $(SOURCES:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJECTS)
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJECTS)

$(OBJECTS):$(SOURCES) $(INCLUDES)
	$(CC) -c $(CCFLAGS) $(SOURCES)

build:$(TARGET)
	mkdir $(BUILDPATH)
	cp $(TARGET) $(BUILDPATH)

clean:
	rm -f $(TARGET) $(OBJECTS)

clean_build:
	rm -rf $(BUILDPATH)

clean_all:
	make clean
	make clean_build
