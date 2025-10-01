#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "coord_query.h"
#include "coord_core.c"

// Calculate the distance between two points on a sphere (Earth), from their latitutde and longitude.
static inline double haversine(double lat1, double lon1, double lat2, double lon2) {
	const double R = 6371000.0; // Earth radius in meters

	// Distances between latitudes and lonitudes
	const double delta_lat = (lat2 - lat1);
	const double delta_lon = (lon2 - lon1);

	// Latitudes as radians
	const double phi1 = lat1 * M_PI / 180.0;
	const double phi2 = lat2 * M_PI / 180.0;

	// Finding a central angle (theta) between latitude points and longitude points
	const double theta_lat = delta_lat / R;
	const double theta_lon = delta_lon / R;

	// Calculate haversine of the two theta values
	const double hav_delta_lat = sin(theta_lat * 0.5) * sin(theta_lat * 0.5);
	const double hav_delta_lon = sin(theta_lon * 0.5) * sin(theta_lon * 0.5);

	// Calculate hav(theta)
	const double hav_theta = hav_delta_lat + cos(phi1) * cos(phi2) * hav_delta_lon;

	const double distance = (2.0 * R) * asin(sqrt(hav_theta));
	return distance;
}

const struct record *lookup_naive_haversine(struct naive_data *data, double lon, double lat) {
  if (!data || !data->record || data->n <= 0) {      
		return NULL;
  }

	int best_index = 0;
	double best_distance = DBL_MAX;

	// Linear search, memoizing the best result
	for (int i = 0; i < data->n; ++i) {
		double distance = haversine(lat, lon, data->record[i].lat, data->record[i].lon);
		if (distance < best_distance) {
			best_distance = distance;
			best_index = i;
		}
	}
	return &data->record[best_index];
}

int main(int argc, char **argv) {
	return coord_query_loop(argc, argv,
							(mk_index_fn)mk_naive,
							(free_index_fn)free_naive,
							(lookup_fn)lookup_naive_haversine);
}