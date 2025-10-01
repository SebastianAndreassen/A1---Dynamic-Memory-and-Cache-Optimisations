#include <stdlib.h>
#include <assert.h>

#include "record.h"
#include "index_core.h"

int cmp_index_record(const void *a, const void *b) {
	const struct index_record *ia = a, *ib = b;
	if (ia->osm_id < ib->osm_id) {
		return -1;
	}
	if (ia->osm_id > ib->osm_id) {
		return 1;
	}
	if (ia->record < ib->record) {
		return -1;
	}
	if (ia->record > ib->record) {
		return 1;
	}
	return 0;
}

struct indexed_data *mk_indexed(struct record *rs, int n) {
    // Create an index_record
    struct indexed_data *idata = malloc(sizeof(struct indexed_data));
    assert(idata != NULL);
    idata->n = n;
    idata->irecord = n ? malloc((size_t)n * sizeof *idata->irecord) : NULL;
    // Asign values

    for (int i = 0; i < n; ++i) {
        idata->irecord[i].osm_id = rs[i].osm_id;
        idata->irecord[i].record = &rs[i]; // pointer to that element
    }
    return idata;
}

struct indexed_data *mk_indexed_sorted(struct record *rs, int n) {
	struct indexed_data *idx = mk_indexed(rs, n); // builds unsorted
	qsort(idx->irecord, (size_t)idx->n, sizeof idx->irecord[0], cmp_index_record);
	// sanity check: verify sorted
	for (int i = 1; i < idx->n; ++i) {
		assert(idx->irecord[i - 1].osm_id <= idx->irecord[i].osm_id);
	}
	return idx;
}

void free_indexed(struct indexed_data *data) {
    if (!data) {
        return;
    }
    free(data->irecord);
    free(data);
}