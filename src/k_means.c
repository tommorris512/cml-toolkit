#include "k_means.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>


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
 * Assigns each data point in a given sample to the KMeans model's closest centroid.
 * Updates the labels array with the index of the closest centroid for each data point.
 *
 * Iterates over each data point with an initially infinite minimal distance.
 * The Euclidean distance is calculated between the data point and each centroid to locate the minima.
 * The closest centroid's label is assigned to the data points corresponding position in the labels array.
 */
static void assign_labels(const KMeans* km, double** X, int num_samples, int* labels) {
    for (int i = 0; i < num_samples; i++) {
        // Initially set the minimum distance to the maximum double value (acting as infinity).
        double min_distance = DBL_MAX;
        int label = 0;

        // Obtain the minimal cluster index and assign it as the data point's label.
        for (int j = 0; j < km->k; j++) {
            double distance = calculate_distance(X[i], km->centroids[j], km->num_variables);

            if (distance < min_distance) {
                min_distance = distance;
                label = j;
            }
        }

        labels[i] = label;
    }
}


/*
 * Updates the centroids of the KMeans model based on the current cluster assignments.
 * Calculates the new centroids by averaging the data points assigned to a particular cluster.
 * Returns EXIT_SUCCESS on completion, and EXIT_FAILURE otherwise.
 * 
 * Dynamically allocates memory for a summation array that stores the accumulation of a cluster's data points.
 * Also dynamically allocates memory for a counting array that holds the quantity the data points belonging to each cluster.
 * At any point, if a dynamic allocation fails, the allocated memory is freed and the functions returns a failure exit value.
 * Iterates over the samples and accumulates the data points and the quantity of them belonging to each cluster.
 * Computes the new centroid locations by averaging the accumulated summations according to the corresponding quantities.
 * Updates the centroid locations in the KMeans model with these new computed locations.
 * Once the update has completed, the allocated memory is freed and the function returns a success exit value.
 */
static int update_centroids(KMeans* km, double** X, int num_samples, const int* labels) {
    // Allocate memory for the summation array.
    double** sum = (double**) calloc(km->k, sizeof(double*));

    if (sum == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for cluster summation array\n");
        free(sum);

        return EXIT_FAILURE;
    }

    // Allocate memory for the quantity array.
    int* count = (int*) calloc(km->k, sizeof(int));

    if (count == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for cluster quantity array\n");
        free(sum);
        free(count);

        return EXIT_FAILURE;
    }

    // Populate the summation array with zeroed data point for each cluster.
    for (int i = 0; i < km->k; i++) {
        sum[i] = (double*) calloc(km->num_variables, sizeof(double));

        if (sum[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for sum %d\n", i);

            for (int j = 0; j < i; j++) {
                free(sum[j]);
            }

            free(sum);
            free(count);

            return EXIT_FAILURE;
        }
    }

    // Iterate over each data point to accumulate the cluster summations.
    for (int i = 0; i < num_samples; i++) {
        int label = labels[i];

        for (int j = 0; j < km->num_variables; j++) {
            sum[label][j] += X[i][j];
        }

        count[label]++;
    }

    // Update the centroids by calculating and assigning their new locations.
    for (int i = 0; i < km->k; i++) {
        if (count[i] > 0) {
            for (int j = 0; j < km->num_variables; j++) {
                km->centroids[i][j] = sum[i][j] / count[i];
            }
        }
    }

    // Free the dynamically allocated memory used.
    for (int i = 0; i < km->k; i++) {
        free(sum[i]);
    }

    free(sum);
    free(count);

    return EXIT_SUCCESS;
}


/*
 * Creates a new KMeans for a specified number of clusters and features.
 * Returns a pointer to a new KMeans on success and NULL on failure.
 *
 * Dynamically allocates memory for a KMeans struct and the subsequent centroid array of points.
 * Each centroid's initial value is randomly set within the provided range.
 * A NULL is returned if any dynamic allocations fails, with any already allocated memory freed.
 * A NULL is also returned if the initial centroid range provided is non-positive.
 */
KMeans* create_k_means(int k, int num_variables, double initial_centroid_range) {
    // Ensure the initial range is positive.
    if (initial_centroid_range <= 0) {
        fprintf(stderr, "Error: Initial centroid range must be a positive value\n");
        return NULL;
    }

    // Seed the random generator using the current system time.
    srand(time(NULL));

    // Allocate memory for a KMeans model.
    KMeans* km = (KMeans*) malloc(sizeof(KMeans));

    if (km == NULL) {
        fprintf(stderr, "Error: Failed to allocate sufficient memory for KMeans model\n");
        return NULL;
    }

    // Allocate memory for the KMeans model's centroids.
    km->centroids = (double**) malloc(k * sizeof(double*));

    if (km->centroids == NULL) {
        fprintf(stderr, "Error: Failed to allocate sufficient memory for KMeans model\n");
        free(km);

        return NULL;
    }

    // Allocate each centroid as a random point of the specified dimension.
    for (int i = 0; i < k; i++) {
        km->centroids[i] = (double*) malloc(num_variables * sizeof(double));

        if (km->centroids[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate sufficient memory for KMeans model\n");

            for (int j = 0; j < i; j++) {
                free(km->centroids[i]);
            }

            free(km->centroids);
            free(km);

            return NULL;
        }

        // Initialise each centroid as a random point within the specified range.
        for (int j = 0; j < num_variables; j++) {
            km->centroids[i][j] = (rand() / (double) RAND_MAX) * (2 * initial_centroid_range) - initial_centroid_range;
        }
    }

    // Assign the remaning parameters to the KMeans model and return it.
    km->k = k;
    km->num_variables = num_variables;

    return km;
}


/*
 * Fits (trains) the KMeans model to the given data points.
 * Performs the k-means clustering algorithm for a specified number of iterations.
 *
 * Dynamically allocates memory for the labels array, which stores the cluster assignment for each data point.
 * Each iteration assigns the data point to the nearest centroid and then updates the centroids based on the current cluster assigments.
 * At any point, if a dynamic allocation fails, the allocated memory is freed and the functions exits.
 * Upon successful fitting, the dynamically allocated labels array is freed.
 */
void fit_k_means(KMeans* km, double** X, int num_samples, int num_iterations) {
    // Allocate memory for labels to store the cluster assignment to each data point.
    int* labels = (int*) malloc(num_samples * sizeof(int));

    if (labels == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for labels\n");
        return;
    }

    // Perform the k-means clustering algorithm for the specified iteration count.
    for (int iteration = 0; iteration < num_iterations; iteration++) {
        // Assign each data point to its nearest centroid's label.
        assign_labels(km, X, num_samples, labels);

        // Update the centroids on the new cluster assignments and exit if this fails.
        if (update_centroids(km, X, num_samples, labels) != EXIT_SUCCESS) {
            free(labels);
            return;
        }
    }

    free(labels);
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

    // Obtain the minimal cluster index.
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
    if (km == NULL) return;

    for (int i = 0; i < km->k; ++i) {
        free(km->centroids[i]);
    }
    
    free(km->centroids);
    free(km);
}