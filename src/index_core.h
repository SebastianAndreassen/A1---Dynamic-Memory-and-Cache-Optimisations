#ifndef INDEX_CORE_H
#define INDEX_CORE_H

#include <stdint.h>

struct index_record {
  int64_t osm_id;
  const struct record *record;
};

struct indexed_data {
  struct index_record *idx_record;
	int n;
};

int cmp_index_record(const void *a, const void *b);
struct indexed_data *mk_indexed(struct record *record, int n);
struct indexed_data *mk_indexed_sorted(struct record *record, int n);
void free_indexed(struct indexed_data *idx_data);

#endif