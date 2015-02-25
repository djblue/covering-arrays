CC = gcc
CFLAGS = -std=c99 -O3
LFLAGS = -lm

.PHONY: clean zip

# build project
all: checker-serial checker-parallel

checker-serial: main.c vector.c vector.h
	$(CC) $(CFLAGS) -o checker-serial main.c vector.c $(LFLAGS)
	
checker-parallel: main.c vector.c vector.h
	$(CC) $(CFLAGS) -o checker-parallel main.c vector.c $(LFLAGS) -fopenmp

# zip project directory using the scheme
# yourFirstName-yourLastName.zip
zip:
	git archive --format zip master -o Abdullah-Badahdah.zip

# run all the tests
test: all
	sh test.sh

clean:
	rm -f checker-parallel checker-serial
