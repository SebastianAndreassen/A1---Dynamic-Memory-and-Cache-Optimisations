#include <stdlib.h>

#include "id_query.h"
#include "record.h"
#include "index_core.h"

const struct record *lookup_binsort_manual(const struct indexed_data *data, const int64_t needle) {
	if (!data || !data->idx_record || data->n <= 0) {
		return NULL;
	}

	size_t lo = 0;
	size_t hi = (size_t)data->n; // search in [lo, hi)

	while (lo < hi) {
		size_t mid = lo + (hi - lo) / 2; // avoid overflow
		int64_t key = data->idx_record[mid].osm_id;

		if (key < needle) {
			lo = mid + 1;
		} else if (key > needle) {
			hi = mid; // shrink to [lo, mid)
		} else {
			return data->idx_record[mid].record; // found
		}
	}
	return NULL; // not found
}

int main(int argc, char **argv) {
	return id_query_loop(argc, argv,
                       (mk_index_fn)mk_indexed_sorted,
                       (free_index_fn)free_indexed,
                       (lookup_fn)lookup_binsort_manual);
}