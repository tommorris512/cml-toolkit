#include "k_means.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

/*
 * Helper function to calculate the Euclidean distance between two points of a given dimensionality.
 * Returns the absolute distance between the two points.
 *
 * Sums the squared difference of each dimension and returns the square root of that summation.
 */
static double calculate_distance(double* a, double* b, int dimensions) {
    double sum = 0.0;

    for (int i = 0; i < dimensions; ++i) {
        sum += pow(a[i] - b[i], 2);
    }

    return sqrt(sum);
}

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
 * Predicts the cluster for a data point based on the KMeans model.
 * Returns the index of the cluster nearest the data point.
 *
 * Iterates over all centroidsto find the closest centroid to the supplied data point.
 * The closest centroid is found by calculating the Euclidean distance between each centroid and the data point, and selecting the minima.
 * The index of the centroid with minimal distance from the data point is returned.
 */
int predict_k_means(KMeans* km, double* X) {
    // Initially set the minimum distance to the maximum double value (acting as infinity).
    double min_distance = DBL_MAX;
    int cluster = 0;

    // Obtain the minimal cluster index
    for (int i = 0; i < km->k; i++) {
        double distance = calculate_distance(X, km->centroids[i], km->num_variables);

        if (distance < min_distance) {
            min_distance = distance;
            cluster = i;
        }
    }

    return cluster;
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
