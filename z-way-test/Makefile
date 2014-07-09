### Variables ###
TARGET = z-way-test

ZWAY_LIB = libzway
ZWAY_LIB_DIR = libzway
ZWAY_LIB_SRC_DIR = ../z-way

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))

INCLUDES = -I$(ZWAY_LIB_DIR)

LIBDIR = -L$(ZWAY_LIB_DIR)
LIB_SO = -lzway
LIB_A = $(ZWAY_LIB_DIR)/libzway.a

#LDFLAGS += -static
LIBS += -lpthread -lxml2 -lz -lm -lcrypto -larchive

### Targets ###

all: $(TARGET)

run: run_so run_a

run_a:
	./$(TARGET)_a

run_so:
	(export LD_LIBRARY_PATH=$(ZWAY_LIB_DIR):$(LD_LIBRARY_PATH); export DYLD_LIBRARY_PATH=$(ZWAY_LIB_DIR):$(DYLD_LIBRARY_PATH); ./$(TARGET)_so)

$(TARGET): $(TARGET)_so $(TARGET)_a

$(TARGET)_a: $(OBJECTS)
	$(CC) $(LIBDIR) $(TARGET_LIBDIR) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< $(LIB_A) $(LIBS)

$(TARGET)_so: $(OBJECTS)
	$(CC) $(LIBDIR) $(TARGET_LIBDIR) $(LDFLAGS) $(TARGET_ARCH) -o $@ $< $(LIB_SO) $(LIBS)

%.o: %.c $(ZWAY_LIB_DIR)/$(ZWAY_LIB).a $(ZWAY_LIB_DIR)/$(ZWAY_LIB).so
	$(CC) $(INCLUDES) $(TARGET_INCLUDES) $(CFLAGS) $(TARGET_ARCH) -c $< -o $@

$(ZWAY_LIB_DIR)/$(ZWAY_LIB).so $(ZWAY_LIB_DIR)/$(ZWAY_LIB).a:
	@if [ -f $(ZWAY_LIB_SRC_DIR)/libzway.tgz ]; then tar -zxf $(ZWAY_LIB_SRC_DIR)/libzway.tgz; else echo "You need to copy the z-way library manually to $(ZWAY_LIB_DIR) folder"; exit 1; fi

clean:
	rm -f *.o $(TARGET)_so $(TARGET)_a

full_clean: clean
	rm -Rf $(ZWAY_LIB_DIR)
	
.PHONY: all clean $(TARGET) run run_a run_so full_clean
