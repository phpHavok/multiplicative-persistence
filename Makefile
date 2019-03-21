CCFLAGS := -ansi -Wall -pedantic
LDFLAGS := -lgmp

mp-search: mp-search.c
	gcc $< $(CCFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f mp-search

.PHONY: clean
