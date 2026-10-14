#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
//  the points used in the visualizer were used in such a way that it was generated 
//  randomly at first in vs code the uploaded from a point.txt file. the txt file is attached here along. 
typedef struct {
    double x, y;
} pnt;

double get_dist(pnt p1, pnt p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
void brute_force(pnt pts[], int tot, double *min_d, pnt *a, pnt *b) {
    *min_d = DBL_MAX;
    for (int i = 0; i < tot; i++) {
        for (int j = i + 1; j < tot; j++) {
            double d = get_dist(pts[i], pts[j]);
            if (d < *min_d) {
                *min_d = d;
                *a = pts[i];
                *b = pts[j];
            }
        }
    }
}

int sort_x(const void *p1, const void *p2) {
    pnt *a = (pnt *)p1;
    pnt *b = (pnt *)p2;
    return (a->x > b->x) - (a->x < b->x);
}

int sort_y(const void *p1, const void *p2) {
    pnt *a = (pnt *)p1;
    pnt *b = (pnt *)p2;
    return (a->y > b->y) - (a->y < b->y);
}
void dc_rec(pnt pts_x[], pnt pts_y[], int tot, double *min_d, pnt *a, pnt *b) {
    if (tot <= 3) {
        brute_force(pts_x, tot, min_d, a, b);
        return;
    }

    int mid = tot / 2;
    pnt mid_p = pts_x[mid];

    pnt *l_y = (pnt *)malloc(mid * sizeof(pnt));
    pnt *r_y = (pnt *)malloc((tot - mid) * sizeof(pnt));
    int l_cnt = 0, r_cnt = 0;

    for (int i = 0; i < tot; i++) {
        if (pts_y[i].x < mid_p.x || (pts_y[i].x == mid_p.x && pts_y[i].y <= mid_p.y)) {
            if (l_cnt < mid) l_y[l_cnt++] = pts_y[i];
            else r_y[r_cnt++] = pts_y[i];
        } else {
            if (r_cnt < tot - mid) r_y[r_cnt++] = pts_y[i];
            else l_y[l_cnt++] = pts_y[i];
        }
    }

    double d1, d2;
    pnt a1, b1, a2, b2;

    dc_rec(pts_x, l_y, mid, &d1, &a1, &b1);
    dc_rec(pts_x + mid, r_y, tot - mid, &d2, &a2, &b2);

    if (d1 < d2) {
        *min_d = d1; *a = a1; *b = b1;
    } else {
        *min_d = d2; *a = a2; *b = b2;
    }

    pnt *strip = (pnt *)malloc(tot * sizeof(pnt));
    int s_size = 0;
    for (int i = 0; i < tot; i++) {
        if (fabs(pts_y[i].x - mid_p.x) < *min_d) {
            strip[s_size++] = pts_y[i];
        }
    }

    for (int i = 0; i < s_size; i++) {
        for (int j = i + 1; j < s_size && (strip[j].y - strip[i].y) < *min_d; j++) {
            double d = get_dist(strip[i], strip[j]);
            if (d < *min_d) {
                *min_d = d;
                *a = strip[i];
                *b = strip[j];
            }
        }
    }

    free(l_y);
    free(r_y);
    free(strip);
}

void divide_and_conquer(pnt pts[], int tot, double *min_d, pnt *a, pnt *b) {
    pnt *pts_x = (pnt *)malloc(tot * sizeof(pnt));
    pnt *pts_y = (pnt *)malloc(tot * sizeof(pnt));
    for (int i = 0; i < tot; i++) {
        pts_x[i] = pts[i];
        pts_y[i] = pts[i];
    }

    qsort(pts_x, tot, sizeof(pnt), sort_x);
    qsort(pts_y, tot, sizeof(pnt), sort_y);

    dc_rec(pts_x, pts_y, tot, min_d, a, b);

    free(pts_x);
    free(pts_y);
}

int main() {
    int N;
    printf("Enter N: ");
    if (scanf("%d", &N) != 1) return 0;

    if (N < 2) {
        printf("Error: N must be at least 2.\n");
        return 0;
    }

    pnt *pts = (pnt *)malloc(N * sizeof(pnt));
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        pts[i].x = ((double)rand() / RAND_MAX) * 100.0;
        pts[i].y = ((double)rand() / RAND_MAX) * 100.0;
    }

    pnt bf_a, bf_b, dc_a, dc_b;
    double bf_d, dc_d;

    clock_t t1 = clock();
    brute_force(pts, N, &bf_d, &bf_a, &bf_b);
    clock_t t2 = clock();
    double bf_time = ((double)(t2 - t1)) * 1000.0 / CLOCKS_PER_SEC;

    t1 = clock();
    divide_and_conquer(pts, N, &dc_d, &dc_a, &dc_b);
    t2 = clock();
    double dc_time = ((double)(t2 - t1)) * 1000.0 / CLOCKS_PER_SEC;

    printf("%.6f (%.2f, %.2f) and (%.2f, %.2f)\n", bf_d, bf_a.x, bf_a.y, bf_b.x, bf_b.y);
    printf("%.6f (%.2f, %.2f) and (%.2f, %.2f)\n", dc_d, dc_a.x, dc_a.y, dc_b.x, dc_b.y);
    printf("Brute-Force: %.4f ms | Divide-and-Conquer: %.4f ms\n", bf_time, dc_time);

    free(pts);
    return 0;
}