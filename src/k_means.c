#include "k_means.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>


/*
 * Creates a new KMeans for a specified number of clusters and features.
 * Returns a pointer to a new KMeans on success and NULL on failure.
 *
 * Dynamically allocates memory for a KMeans struct and the subsequent centroid array of points (with each point zeroed).
 * A NULL is returned if any dynamic allocations fails, with any already allocated memory freed.
 */
KMeans* create_k_means(int k, int num_variables) {
    KMeans* km = (KMeans*) malloc(sizeof(KMeans));

    if (km == NULL) {
        fprintf(stderr, "Error: Failed to allocate sufficient memory for KMeans model\n");
        return NULL;
    }

    km->centroids = (double**) malloc(k * sizeof(double*));

    if (km->centroids == NULL) {
        free(km);
        fprintf(stderr, "Error: Failed to allocate sufficient memory for KMeans model\n");
        return NULL;
    }

    for (int i = 0; i < k; i++) {
        km->centroids[i] = (double*) calloc(num_variables, sizeof(double));

        if (km->centroids[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(km->centroids[i]);
            }

            free(km->centroids);
            free(km);

            fprintf(stderr, "Error: Failed to allocate sufficient memory for KMeans model\n");
            return NULL;
        }
    }

    km->k = k;
    km->num_variables = num_variables;

    return km;
}

/*
 * Frees the dynamically allocated memory used by the KMeans model.
 *
 * Ensures that the model is non-null and deallocates its centroid array, followed by the model itself.
 */
void free_k_means(KMeans* km) {
    for (int i = 0; i < km->k; ++i) {
        free(km->centroids[i]);
    }
    
    free(km->centroids);
    free(km);
}
