#ifndef SRC_MYASSERT_H_
#define SRC_MYASSERT_H_

/*
 * Define constants for test success and failure.
 */
#define ASSERTION_FAILURE 0
#define TEST_SUCCESS 1

/*
 * Define behaviour for the assert macro function on an expression.
 */
#define assert(expr) do {\
        if (!(expr)) {\
            printf("Assertion failed in test %s (%s line %d)\n", __FUNCTION__, __FILE__, __LINE__);\
            return ASSERTION_FAILURE;\
        }\
} while(0)

#endif /* SRC_MYASSERT_H_ */
