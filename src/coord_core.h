#ifndef COORD_CORE_H
#define COORD_CORE_H

struct naive_data {
    struct record *record;
    int n;
};

struct naive_data *mk_naive(struct record *record, int n);

void free_naive(struct naive_data *data);

#endif