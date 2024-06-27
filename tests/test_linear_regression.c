#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "assert.h"
#include "linear_regression.h"

/*
 * The default independent variable quantity to use during tests.
 */
#define DEFAULT_NUM_VARIABLES 4

/*
 * The model to use during tests.
 */
static LinearRegression* lr;

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
    lr = new_linear_regression(DEFAULT_NUM_VARIABLES);
    total_count++;
}

/*
 * Teardown function to run after each test.
 */
void teardown() {
    free_linear_regression(lr);
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
 * Checks that the LinearRegression constructor returns a non-null pointer.
 */
int new_linear_regression_is_non_null() {
    assert(lr != NULL);
    return TEST_SUCCESS;
}

/*
 * Checks that the LinearRegression constructor initializes weights to zero.
 */
int new_linear_regression_initializes_weights_to_zero() {
    for (int i = 0; i < DEFAULT_NUM_VARIABLES; i++) {
        assert(fabs(lr->weights[i]) < EPSILON);
    }
    return TEST_SUCCESS;
}

/*
 * Checks that the LinearRegression model can train on simple data and make accurate predictions.
 */
int linear_regression_can_train_and_predict() {
    double X[4][DEFAULT_NUM_VARIABLES] = {
        {1.0, 2.0, 3.0, 4.0},
        {2.0, 3.0, 4.0, 5.0},
        {3.0, 4.0, 5.0, 6.0},
        {4.0, 5.0, 6.0, 7.0}
    };
    double y[4] = {10.0, 14.0, 18.0, 22.0};
    double* X_ptr[4];
    for (int i = 0; i < 4; i++) {
        X_ptr[i] = X[i];
    }

    train_linear_regression(lr, X_ptr, y, 4, 0.01, 1000);

    double test_sample[DEFAULT_NUM_VARIABLES] = {5.0, 6.0, 7.0, 8.0};
    double prediction = predict_linear_regression(lr, test_sample);
    double expected = 26.0;

    assert(fabs(prediction - expected) < EPSILON);
    return TEST_SUCCESS;
}

/*
 * Checks that freeing a NULL LinearRegression model does not cause errors.
 */
int free_null_linear_regression() {
    free_linear_regression(NULL);
    return TEST_SUCCESS;
}

/*
 * Main function to run each of the defined LinearRegression tests.
 */
int main() {
    printf("Running Linear Regression tests...\n");

    // Run the tests
    run_test(new_linear_regression_is_non_null);
    run_test(new_linear_regression_initializes_weights_to_zero);
    run_test(linear_regression_can_train_and_predict);
    run_test(free_null_linear_regression);

    printf("----------------\n");
    printf("Linear Regression Tests complete: %d / %d tests successful.\n", success_count, total_count);
    printf("----------------\n");

    return 0;
}
