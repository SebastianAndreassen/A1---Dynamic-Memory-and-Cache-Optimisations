#include <stdlib.h>
#include <assert.h>

#include "record.h"
#include "coord_core.h"

struct naive_data *mk_naive(struct record *record, int n) {
	// Allocate memory for the data structure and ensure it succeeds
	struct naive_data *nd = malloc(sizeof(struct naive_data));
	assert(nd != NULL);

	// Assign values
	nd->record = record;
	nd->n = n;

	return nd;
}

void free_naive(struct naive_data *data) {
	free(data);
}