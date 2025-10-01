#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <stddef.h>

#include "record.h"
#include "id_query.h"


struct indexed_data {
struct index_record *irs;
int n;
};

struct index_record {
int64_t osm_id;
const struct record *record;
};

int compare(const void* a, const void* b) {
   return (*(int64_t*)a - *(int64_t*)b);
}

struct indexed_data* mk_indexed(struct record* rs, int n){
  
  struct indexed_data* bigGeo = malloc(sizeof(struct indexed_data));
  bigGeo->n=n;
  bigGeo->irs = n ? malloc((size_t)n * sizeof *bigGeo->irs) : NULL;

  for(int i=0;i<n;i++){
    bigGeo->irs[i].osm_id = rs[i].osm_id;
    bigGeo->irs[i].record=&rs[i];
  }
  
  qsort(bigGeo->irs,n,sizeof(struct indexed_data),compare);
  return bigGeo;
}



void free_indexed(struct indexed_data* data){
  free(data->irs);
  free(data);
}



const struct record* lookup_indexed(struct indexed_data *data,int64_t needle){
    

  struct index_record *pitem= bsearch(&needle,data->irs,data->n,sizeof(struct index_record),compare);

  if(pitem==NULL){
    return(0);
  }
    
  return pitem->record;
}



int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
