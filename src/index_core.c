#include <stdlib.h>
#include <assert.h>

#include "record.h"
#include "index_core.h"

int cmp_index_record(const void *a, const void *b) {
	const struct index_record *idx_a = a, *idx_b = b;
	if (idx_a->osm_id < idx_b->osm_id) {
		return -1;
	}
	if (idx_a->osm_id > idx_b->osm_id) {
		return 1;
	}
	if (idx_a->record < idx_b->record) {
		return -1;
	}
	if (idx_a->record > idx_b->record) {
		return 1;
	}
	return 0;
}

struct indexed_data *mk_indexed(struct record *record, int n) {
	// Create an index_record
	struct indexed_data *idata = malloc(sizeof(struct indexed_data));
	assert(idata != NULL);
	idata->n = n;
	idata->idx_record = n ? malloc((size_t)n * sizeof *idata->idx_record) : NULL;
	// Asign values

	for (int i = 0; i < n; ++i) {
		idata->idx_record[i].osm_id = record[i].osm_id;
		idata->idx_record[i].record = &record[i]; // pointer to that element
	}
	return idata;
}

struct indexed_data *mk_indexed_sorted(struct record *record, int n) {
	struct indexed_data *idx = mk_indexed(record, n); // builds unsorted
	qsort(idx->idx_record, (size_t)idx->n, sizeof idx->idx_record[0], cmp_index_record);
	// sanity check: verify sorted
	for (int i = 1; i < idx->n; ++i) {
		assert(idx->idx_record[i - 1].osm_id <= idx->idx_record[i].osm_id);
	}
	return idx;
}

void free_indexed(struct indexed_data *idx_data) {
    if (!idx_data) {
        return;
    }
    free(idx_data->idx_record);
    free(idx_data);
}