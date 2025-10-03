#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "coord_query.h"
#include "coord_core.c"

// Calculate distance between two points, using squared distances
static inline double euclidean(double lat1, double lon1, double lat2, double lon2) {
  double dlat = lat2 - lat1;
  double dlon = lon2 - lon1;
  return sqrt(dlat*dlat + dlon*dlon);
}

const struct record *lookup_naive_euclidean(struct naive_data *data, double lon, double lat) {
  if (!data || !data->record || data->n <= 0) {
    return NULL;
  }

  int best_index = 0;
  double best_distance = DBL_MAX;

  // Linear search, memoizing the best result 
  for (int i = 0; i < data->n; ++i) {
    double distance = euclidean(lat, lon, data->record[i].lat, data->record[i].lon);
    if (distance < best_distance) {
      best_distance = distance;
      best_index = i;
    }
  }
  return &data->record[best_index];
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv, (mk_index_fn)mk_naive, (free_index_fn)free_naive, (lookup_fn)lookup_naive_euclidean);
}