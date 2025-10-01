#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

struct naive_data {
  struct record *rs;
  int n;
};

// Make naive_data struct from a record struct and an integer
struct naive_data* mk_naive(struct record* rs, int n) {
  // Allocate memory for the data structure and ensure it succeeds 
  struct naive_data* nd = malloc(sizeof(struct naive_data));
  assert(nd != NULL);

  // Assign values
  nd->rs = rs;
  nd->n = n;

  return nd;
}

// Free memory allocated for the naive_data struct
void free_naive(struct naive_data* data) {
  free_records(data->rs, data->n);
  free(data);
}

// Linear search through naive_data to find the target id
const struct record* lookup_naive(const struct naive_data *data, int64_t needle) {
  if (!data || !data->rs) {
    return NULL;
  }

  // Linear search
  for (int i = 0; i < data->n; i++) {
    if (data->rs[i].osm_id == needle) {
      return &data->rs[i];
    }
  }
  return NULL;
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_naive,
                    (free_index_fn)free_naive,
                    (lookup_fn)lookup_naive);
}
