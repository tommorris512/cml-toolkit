#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

/*
 * Define a typed struct to encapsulate LinearRegression models.
 */
typedef struct {
    double* weights;
    int num_variables;
} LinearRegression;


/* FUNCTION PROTOTYPES */

/*
 * Creates a new LinearRegression for a specified number of variables.
 * Returns a pointer to a new LinearRegression on success and NULL on failure.
 */
LinearRegression* new_linear_regression(int num_variables);

/*
 * Trains the given LinearRegression model based on a series of samples and their accompanying target values.
 * The impact each sample has on the LinearRegression model is controlled by the learning_rate parameter.
 */
void train_linear_regression(LinearRegression* lr, double** X, double* y, int num_samples, double learning_rates, int num_iterations);

/*
 * Predicts the target value of the dependent variable based on the independent variables supplied.
 * Returns the predicted value based on the model's weights.
 */
double predict_linear_regression(LinearRegression* lr, double* X);

/*
 * Frees the dynamically allocated memory used by the LinerRegression model.
 */
void free_linear_regression(LinearRegression* lr);

#endif /* For LINEAR_REGRESSION_H */