#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "id_query.h"
#include "record.h"
#include "index_core.h"

const struct record *lookup_indexed(const struct indexed_data *data, int64_t needle) {
    for (int i = 0; i < data->n; ++i) {
        if (data->irecord[i].osm_id == needle) {
            return data->irecord[i].record;
        }
    }       
    return NULL;
}

int main(int argc, char **argv) {
    return id_query_loop(argc, argv,
                         (mk_index_fn)mk_indexed,
                         (free_index_fn)free_indexed,
                         (lookup_fn)lookup_indexed);
}