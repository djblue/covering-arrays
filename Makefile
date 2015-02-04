CC = gcc
BIN = checker
CFLAGS = -std=c99 -O3
LFLAGS = -lm -fopenmp

.PHONY: clean zip

# build project
all: checker

checker: main.c vector.c vector.h
	$(CC) $(CFLAGS) -o $(BIN) main.c vector.c $(LFLAGS)

# zip project directory using the scheme
# yourFirstName-yourLastName.zip
zip:
	git archive --format zip master -o Abdullah-Badahdah.zip

# run all the tests
test: all
	sh test.sh

clean:
	rm -f $(BIN)
