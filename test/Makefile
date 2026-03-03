CC = gcc
CFLAGS = -Wall -Wextra -I../src -Itest
SRC = ../src/url-parser.c

TESTS = $(wildcard *.test.c)
BINS = $(TESTS:.test.c=)

all: $(BINS)

%: %.test.c $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(SRC)

run: all
	@failed=0; \
	for test in $(BINS); do \
		echo "Running $$test..."; \
		./$$test || failed=1; \
	done; \
	if [ $$failed -eq 1 ]; then \
		echo "Some tests failed!"; \
		exit 1; \
	fi
	@echo "All tests passed!"

clean:
	rm -f $(BINS)

.PHONY: all run clean
