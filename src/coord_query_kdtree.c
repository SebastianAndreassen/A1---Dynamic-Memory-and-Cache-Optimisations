#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "record.h"
#include "coord_query.h"

struct kd_node {
	struct record *record;     // pointer into the array
	int axis;                  // 0: lon, 1: lat
	struct kd_node *left, *right;
};

struct kd_tree {
	struct kd_node *root;
	int n; // number of nodes
};

/* comparators for qsort */
static int cmp_lon(const void *pa, const void *pb) {
	const struct record *a = pa, *b = pb;
	return (a->lon > b->lon) - (a->lon < b->lon);
}

static int cmp_lat(const void *pa, const void *pb) {
	const struct record *a = pa, *b = pb;
	return (a->lat > b->lat) - (a->lat < b->lat);
}

static struct kd_node *mk_node(struct record *record, int n, int depth) {
	if (n <= 0) {
		return NULL;
	}
	int axis = depth % 2;

	// Sort current slice by axis, then take median
	qsort(record, (size_t)n, sizeof *record, axis ? cmp_lat : cmp_lon);

	int m = n / 2;

	struct kd_node *node = malloc(sizeof *node);
	if (!node) {
		return NULL;
	}
	node->axis = axis;
	node->record = &record[m];
	node->left = NULL;
	node->right = NULL;

	// Left partition: [0, m)
	node->left  = mk_node(record, m, depth + 1);
	// Right partition: (m, n)
	node->right = mk_node(&record[m + 1], n - m - 1, depth + 1);

	return node;
}

struct kd_tree *mk_kdtree(struct record *record, int n) {
	struct kd_tree *tree = malloc(sizeof *tree);
	if (!tree) {
		return NULL;
	}
	tree->n = n;
	tree->root = mk_node(record, n, 0);
	return tree;
}

/* -------- nearest-neighbor search (uses squared distances) -------- */
static inline double euclidean2(double qlon, double qlat, double plon, double plat) {
	double dlon = qlon - plon;
	double dlat = qlat - plat;
	return dlon*dlon + dlat*dlat;
}

static void nn_search(const struct kd_node *node, double qlon, double qlat, const struct record **best, double *bestd2) {
	if (!node) {
		return;
	}
	/* visit current */
	double d2 = euclidean2(qlon, qlat, node->record->lon, node->record->lat);
	if (d2 < *bestd2) {
			*bestd2 = d2;
			*best   = node->record;
	}

	/* choose near/far by which side contains the query */
	double qv = (node->axis == 0) ? qlon : qlat;
	double pv = (node->axis == 0) ? node->record->lon : node->record->lat;

	const struct kd_node *near = (qv < pv) ? node->left  : node->right;
	const struct kd_node *far  = (qv < pv) ? node->right : node->left;

	/* go down the near side first */
	nn_search(near, qlon, qlat, best, bestd2);

	/* prune: explore far side only if hypersphere intersects the split plane */
	double diff = qv - pv;
	double diff2 = diff * diff;        // squared distance to splitting plane
	if (diff2 <= *bestd2) {
		nn_search(far, qlon, qlat, best, bestd2);
	}
}

const struct record *kd_nearest(const struct kd_tree *tree, double qlon, double qlat) {
	if (!tree || !tree->root) {
		return NULL;
	}
	const struct record *best = NULL;
	double best_distance2 = DBL_MAX;
	nn_search(tree->root, qlon, qlat, &best, &best_distance2);
	return best;
}

static void free_kd_subtree(struct kd_node *node) {
	if (!node) return;
	free_kd_subtree(node->left);
	free_kd_subtree(node->right);
	free(node);
}

void free_kdtree(struct kd_tree *tree) {
	if (!tree) return;
	free_kd_subtree(tree->root);
	free(tree);
}

int main(int argc, char** argv) {
	return coord_query_loop(argc, argv,
													(mk_index_fn)mk_kdtree,
													(free_index_fn)free_kdtree,
													(lookup_fn)kd_nearest);
}