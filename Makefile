CCFLAGS := -ansi -Wall -pedantic
LDFLAGS := -lgmp
TARGET := mp-search

$(TARGET): $(TARGET).c
	gcc $< $(CCFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGET)

.PHONY: clean
