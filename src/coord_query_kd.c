#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "coord_query.h"

struct naive_data {
  struct record *rs;
  int n;
};
struct parent{}

struct leaf{
    
    
    struct record
    
}


struct naive_data* mk_kdtree(struct record* rs, int n) {
  
  
    assert(0);
  // TODO
}

void free_Kd(struct naive_data* data) {
  read_records;
  
  free(data);
  // TODO
}

const struct record* lookup_kd(struct naive_data *data, double lon, double lat) {
  assert(0);
  // TODO
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_kdtree,
                          (free_index_fn)free_Kd,
                          (lookup_fn)lookup_kd);
}
