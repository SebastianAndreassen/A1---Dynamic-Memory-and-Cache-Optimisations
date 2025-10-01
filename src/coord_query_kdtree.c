#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "record.h"
#include "coord_query.h"

struct kd_node {
	struct record *record; // pointer into the array
	int axis;          // 0 or 1 / lon or lat
	struct kd_node *left, *right, *parent;
};

struct kd_tree {
	struct kd_node *root;
	int n; // number of nodes
};

struct nearest_neighbor {
	const struct record *best;
	double best_dist;
};

static int cmp_lat(const void *pa, const void *pb) {
	const struct record *a = pa, *b = pb;
	return (a->lat > b->lat) - (a->lat < b->lat);
}

static int cmp_lon(const void *pa, const void *pb) {
	const struct record *a = pa, *b = pb;
	return (a->lon > b->lon) - (a->lon < b->lon);
}

static struct kd_node *mk_node(struct record *record, int n, int depth, struct kd_node *parent) {
	if (n <= 0) {
		return NULL;
	}
		
	int axis = depth % 2;
	// Sort current slice by axis, then take median
	qsort(record, n, sizeof *record, axis ? cmp_lon : cmp_lat);

	int m = n / 2;
	struct kd_node *node = malloc(sizeof *node);
	if (!node) {
			return NULL; // or handle error
	}
	node->axis = axis;
	node->record = &record[m];
	node->parent = parent;
	node->left = node->right = NULL;

	// Left partition: [0, m)
	node->left = mk_node(record, m, depth + 1, node);
	// Right partition: (m, n)
	node->right = mk_node(&record[m + 1], n - m - 1, depth + 1, node);
	return node;
}

struct kd_tree *mk_kdtree(struct record *record, int n) {
	struct kd_tree *tree = malloc(sizeof *tree);
	if (!tree) {
		return NULL;
	}
	tree->n = n;

	tree->root = mk_node(record, n, 0, NULL);
	return tree;
}

void free_kdtree(struct kd_node *node) {
	if (!node) {
		return;
	}
	free_kdtree(node->left);
	free_kdtree(node->right);
	free(node);
}

/* simple Euclidean distance on (lat,lon) in same units */
static inline double euclidean(double qlat, double qlon, double plat, double plon) {
	double dlat = qlat - plat;
	double dlon = qlon - plon;
	return sqrt(dlat*dlat + dlon*dlon);
}

static void nn_search(const struct kd_node *node, double qlat, double qlon, const struct record **best, double *bestd) {
	if (!node) {
		return;
	}

	/* visit current */
	double d = euclidean(qlat, qlon, node->record->lat, node->record->lon);
	if (d < *bestd) { 
		*bestd = d; 
		*best = node->record; 
	}

	/* choose near/far by which side contains the query */
	double qv = (node->axis == 0) ? qlat : qlon;
	double pv = (node->axis == 0) ? node->record->lat : node->record->lon;
	const struct kd_node *near = (qv < pv) ? node->left  : node->right;
	const struct kd_node *far  = (qv < pv) ? node->right : node->left;

	/* go down the near side first */
	nn_search(near, qlat, qlon, best, bestd);

	/* prune: only explore far side if the circle intersects the split plane */
	double diff = fabs(qv - pv);   // |node.point[axis] - query[axis]|
	double radius = *bestd;        // distance(query, closest)
	if (diff <= radius) {
		nn_search(far, qlat, qlon, best, bestd);
	}
}

/* public API */
const struct record *kd_nearest(const struct kd_tree *tree, double qlat, double qlon) {
	if (!tree || !tree->root) {
		return NULL;
	}
	const struct record *best = NULL;
	double best_distance = DBL_MAX;
	nn_search(tree->root, qlat, qlon, &best, &best_distance);
	return best;
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_kdtree,
                          (free_index_fn)free_kdtree,
                          (lookup_fn)kd_nearest);
}