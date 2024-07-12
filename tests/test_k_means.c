#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "assert.h"
#include "k_means.h"

/*
 * The default number of clusters to use during tests.
 */
#define DEFAULT_NUM_CLUSTERS 3

/*
 * The default number of variables (dimensions) to use during tests.
 */
#define DEFAULT_NUM_VARIABLES 2

/*
 * The model to use during tests.
 */
static KMeans* km;

/*
 * The number of tests that succeeded.
 */
static int success_count = 0;

/*
 * The total number of tests run.
 */
static int total_count = 0;

/*
 * Tolerance for floating-point number comparison.
 */
#define EPSILON 1e-6

/*
 * Setup function to run prior to each test.
 */
void setup() {
    km = create_k_means(DEFAULT_NUM_CLUSTERS, DEFAULT_NUM_VARIABLES);
    total_count++;
}

/*
 * Teardown function to run after each test.
 */
void teardown() {
    free_k_means(km);
}

/*
 * This function is called multiple times from main for each user-defined test function.
 */
void run_test(int (*testFunction)()) {
    setup();

    if (testFunction()) success_count++;

    teardown();
}

/* UNIT TESTS */

/*
 * Checks that the KMeans constructor returns a non-null pointer.
 */
int create_k_means_is_non_null() {
    assert(km != NULL);

    return TEST_SUCCESS;
}

/*
 * Checks that the KMeans constructor initializes centroids to zero.
 */
int create_k_means_initializes_centroids_to_zero() {
    for (int i = 0; i < DEFAULT_NUM_CLUSTERS; i++) {
        for (int j = 0; j < DEFAULT_NUM_VARIABLES; j++) {
            assert(fabs(km->centroids[i][j]) < EPSILON);
        }
    }

    return TEST_SUCCESS;
}

/*
 * Checks that the KMeans model can train on simple data and produce reasonable centroids.
 */
int k_means_can_train() {
    double X[6][DEFAULT_NUM_VARIABLES] = {
        {1.0, 2.0},
        {1.5, 1.8},
        {5.0, 8.0},
        {8.0, 8.0},
        {1.0, 0.6},
        {9.0, 11.0}
    };

    double* X_ptr[6];

    for (int i = 0; i < 6; i++) {
        X_ptr[i] = X[i];
    }

    fit_k_means(km, X_ptr, 6, 10);

    // Check that the centroids are updated from the initial zero values.
    for (int i = 0; i < DEFAULT_NUM_CLUSTERS; i++) {
        bool is_updated = false;

        for (int j = 0; j < DEFAULT_NUM_VARIABLES; j++) {
            if (fabs(km->centroids[i][j]) > EPSILON) {
                is_updated = true;
            }
        }

        assert(is_updated);
    }

    return TEST_SUCCESS;
}

/*
 * Checks that the KMeans model can predict the cluster of a data point.
 */
int k_means_can_predict() {
    double X[6][DEFAULT_NUM_VARIABLES] = {
        {1.0, 2.0},
        {1.5, 1.8},
        {5.0, 8.0},
        {8.0, 8.0},
        {1.0, 0.6},
        {9.0, 11.0}
    };

    double* X_ptr[6];

    for (int i = 0; i < 6; i++) {
        X_ptr[i] = X[i];
    }

    fit_k_means(km, X_ptr, 6, 10);

    double test_sample[DEFAULT_NUM_VARIABLES] = {0.0, 0.0};
    int cluster = predict_k_means(km, test_sample);

    // Check that the predicted cluster is valid (within the range of clusters).
    assert(cluster >= 0 && cluster < DEFAULT_NUM_CLUSTERS);

    return TEST_SUCCESS;
}

/*
 * Checks that freeing a NULL KMeans model does not cause errors.
 */
int free_null_k_means() {
    free_k_means(NULL);

    return TEST_SUCCESS;
}

/*
 * Main function to run each of the defined KMeans tests.
 */
int main() {
    printf("Running KMeans tests...\n");

    // Run the tests
    run_test(create_k_means_is_non_null);
    run_test(create_k_means_initializes_centroids_to_zero);
    run_test(k_means_can_train);
    run_test(k_means_can_predict);
    run_test(free_null_k_means);

    printf("----------------\n");
    printf("KMeans Tests complete: %d / %d tests successful.\n", success_count, total_count);
    printf("----------------\n");

    return 0;
}
