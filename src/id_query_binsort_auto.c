#include <stdlib.h>

#include "record.h"
#include "id_query.h"
#include "index_core.h"

static int cmp_needle_recordelem(const void *key, const void *elem) {
  int64_t needle = *(const int64_t*)key;
  const struct index_record *rec = (const struct index_record*)elem;
  if (needle < rec->osm_id) {
    return -1;
  }
  if (needle > rec->osm_id) {
    return 1;
  }
  return 0;
}


const struct record* lookup_binsort_auto(const struct indexed_data *idxdata, const int64_t needle){
  struct index_record *query_item = bsearch(&needle, idxdata->idx_record, idxdata->n, sizeof(struct index_record), cmp_needle_recordelem);
  if (query_item == NULL) {
    return NULL;
  }
  return query_item->record;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed_sorted,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_binsort_auto);
}