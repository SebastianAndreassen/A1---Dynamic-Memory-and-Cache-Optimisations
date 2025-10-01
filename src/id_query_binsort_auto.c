#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

#include "record.h"
#include "id_query.h"
#include "index_core.h"

const struct record* lookup_binsort_auto(struct indexed_data *data, int64_t needle){
  struct index_record *qitem = bsearch(&needle, data->irecord, data->n, sizeof(struct index_record), cmp_index_record);
  if (qitem == NULL) {
    return NULL;
  }
  return qitem->record;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed_sorted,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_binsort_auto);
}
