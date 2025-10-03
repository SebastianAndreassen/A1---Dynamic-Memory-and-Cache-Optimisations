CC := gcc
CFLAGS := -Wall -Wextra -pedantic -std=gnu99 -g -lm
PROGRAMS := id_query_naive id_query_indexed id_query_binsort_auto id_query_binsort_manual coord_query_naive_euclidean coord_query_naive_haversine coord_query_kdtree random_ids
TESTS := ..

# objects used by both id_query_* programs
COMMON_OBJS := record.o id_query.o index_core.o index_core.h

.PHONY: all test clean ../src.zip

all: $(PROGRAMS)

random_ids: random_ids.o record.o
	$(CC) -o $@ $^ $(CFLAGS)

id_query_%: id_query_%.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

id_query.o: id_query.c
	$(CC) -c $< $^ $(CFLAGS)

id_query_binsort_%: id_query_binsort_%.o record.o id_query.o index_core.o
	$(CC) -o $@ $^ $(CFLAGS)

coord_query_naive_%: coord_query_naive_%.o record.o coord_query.o coord_core.h 
	$(CC) -o $@ $^ $(CFLAGS)

coord_query_kdtree: coord_query_kdtree.o record.o coord_query.o 
	$(CC) -o $@ $^ $(CFLAGS)

coord_query.o: coord_query.c
	$(CC) -c $< $^ $(CFLAGS)

record.o: record.c
	$(CC) -c $< $^ $(CFLAGS)

test: $(TESTS)
	@set e; for test in $(TESTS); do echo ./$$test; ./$$test; done

clean:
	rm -rf core *.o $(PROGRAMS)
	rm *.txt

planet-latest-geonames.tsv:
	wget https://github.com/OSMNames/OSMNames/releases/download/v2.0.4/planet-latest_geonames.tsv.gz
	gunzip planet-latest_geonames.tsv.gz

../src.zip:
	make clean
	cd .. && zip src.zip -r src

# -- Compile rule -- #
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@