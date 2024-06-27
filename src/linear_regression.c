#include "linear_regression.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Creates a new LinearRegression for a specified number of variables.
 * Returns a pointer to a new LinearRegression on success and NULL on failure.
 *
 * Dynamically allocates memory for a LinearRegression struct and the subsequent weights array (which is zeroed).
 * A NULL is returned if either of these dynamic allocations fails, with any already allocated memory freed.
 */
LinearRegression* new_linear_regression(int num_variables) {
    LinearRegression* lr = (LinearRegression*) malloc(sizeof(LinearRegression));

    if (lr == NULL) {
        fprintf(stderr, "Error: Failed to allocate sufficient memory for LinearRegression model\n");
        return NULL;
    }

    lr->weights = (double*) calloc(num_variables, sizeof(double));

    if (lr->weights == NULL) {
        free(lr);
        fprintf(stderr, "Error: Failed to allocate sufficient memory for LinearRegression model\n");
        return NULL;
    }

    lr->num_variables = num_variables;

    return lr;
}

/*
 * Trains the given LinearRegression model based on a series of samples and their accompanying target values.
 * The impact each sample has on the LinearRegression model is controlled by the learning_rate parameter.
 *
 * Ensures that the model and sample-target sets are non-null and executes the training.
 * Each sample's target is predicted with the current weights and compared to the actual target.
 * The error between these is then used to update the model's weights based on the gradient delta.
 * The number of iterations used in the gradient descent is also parameterised here.
 */
void train_linear_regression(LinearRegression* lr, double** X, double* y, int num_samples, double learning_rate, int num_iterations) {
    if (lr == NULL || X == NULL || y == NULL) {
        fprintf(stderr, "Error: Null pointer passed to train_linear_regression\n");
        return;
    }

    for (int iteration = 0; iteration < num_iterations; iteration++) {
        for (int i = 0; i < num_samples; i++) {
            double predicted = 0.0;

            // Calculate the model's prediction
            for (int j = 0; j < lr->num_variables; j++) {
                predicted += lr->weights[j] * X[i][j];
            }

            // Compute the error
            double error = predicted - y[i];

            // Update the weights appropriately
            for (int j = 0; j < lr->num_variables; j++) {
                lr->weights[j] -= learning_rate * error * X[i][j];
            }
        }
    }
}

/*
 * Predicts the target value of the dependent variable based on the independent variables supplied.
 * Returns the predicted value based on the model's weights.
 *
 * Ensures that the model and set of independent variables are non-null and executes the prediction.
 * Employs linear regression on the variables using the model's weights to approximate the target value.
 */
double predict_linear_regression(LinearRegression* lr, double* X) {
    if (lr == NULL || X == NULL) {
        fprintf(stderr, "Error: Null pointer passed to predict_linear_regression\n");
        return 0.0;
    }

    double prediction = 0.0;

    for (int i = 0; i < lr->num_variables; i++) {
        prediction += lr->weights[i] * X[i];
    }

    return prediction;
}

/*
 * Frees the dynamically allocated memory used by the LinerRegression model.
 *
 * Ensures that the model is non-null and deallocates its weights array, followed by the model itself.
 */
void free_linear_regression(LinearRegression* lr) {
    if (lr != NULL) {
        free(lr->weights);
        free(lr);
    }
}