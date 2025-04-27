#pragma once

/*
 *********************************************************************************************************
 *
 *                                                 GCD
 *
 * Description: Computes the greatest common divisor of two integers using Euclidean algorithm.
 *
 * Arguments: a, b   The two integers for which to find the GCD.
 *
 * Returns: The greatest common divisor of a and b.
 *
 * Notes: Assumes that b is not 0 when the function is called.
 *********************************************************************************************************
 */
long long gcd(long long a, long long b);

/*
 *********************************************************************************************************
 *
 *                                                 LCM
 *
 * Description: Computes the least common multiple of two integers.
 *
 * Arguments: a, b   The two integers for which to find the LCM.
 *
 * Returns: The least common multiple of a and b.
 *
 * Notes: Uses the GCD function to calculate LCM using the formula lcm(a,b) =
 *a*b/gcd(a,b).
 *********************************************************************************************************
 */
long long lcm(long long a, long long b);

struct rational {
  long long p, q;
};

/*
 *********************************************************************************************************
 *
 *                                           RATIONAL NEW
 *
 * Description: Creates a new rational number with the specified numerator and denominator.
 *
 * Arguments: p   The numerator of the rational number.
 *            q   The denominator of the rational number.
 *
 * Returns: A rational number struct representing p/q.
 *
 * Notes: The rational number is not automatically simplified.
 *********************************************************************************************************
 */
struct rational r_new(long long p, long long q);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL FROM
 *
 * Description: Creates a new rational number from an integer.
 *
 * Arguments: p   The integer to convert to a rational number.
 *
 * Returns: A rational number struct representing p/1.
 *
 * Notes: This is a convenience function for creating rational numbers from
 *integers.
 *********************************************************************************************************
 */
struct rational r_from(long long p);

/*
 *********************************************************************************************************
 *
 *                                       RATIONAL SIMPLIFY
 *
 * Description: Simplifies a rational number to its lowest terms.
 *
 * Arguments: num   Pointer to the rational number to simplify.
 *
 * Returns: void
 *
 * Notes: The function modifies the rational number in-place. If the denominator
 *is negative, the signs of both numerator and denominator are flipped.
 *********************************************************************************************************
 */
void r_simplify(struct rational *num);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL ADD
 *
 * Description: Adds two rational numbers.
 *
 * Arguments: a, b   The two rational numbers to add.
 *
 * Returns: A rational number representing the sum a + b.
 *
 * Notes: The result is automatically simplified.
 *********************************************************************************************************
 */
struct rational r_add(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL SUB
 *
 * Description: Subtracts one rational number from another.
 *
 * Arguments: a, b   Calculate a - b.
 *
 * Returns: A rational number representing the difference a - b.
 *
 * Notes: The result is automatically simplified.
 *********************************************************************************************************
 */
struct rational r_sub(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL MUL
 *
 * Description: Multiplies two rational numbers.
 *
 * Arguments: a, b   The two rational numbers to multiply.
 *
 * Returns: A rational number representing the product a * b.
 *
 * Notes: The result is automatically simplified.
 *********************************************************************************************************
 */
struct rational r_mul(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL DIV
 *
 * Description: Divides one rational number by another.
 *
 * Arguments: a, b   Calculate a / b.
 *
 * Returns: A rational number representing the quotient a / b.
 *
 * Notes: The result is automatically simplified. Assumes b is not zero.
 *********************************************************************************************************
 */
struct rational r_div(struct rational a, struct rational b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL CMP
 *
 * Description: Compares two rational numbers.
 *
 * Arguments: a, b   Pointers to the rational numbers to compare.
 *
 * Returns: -1 if a < b, 0 if a = b, 1 if a > b.
 *
 * Notes: Comparison is done by cross multiplication to avoid precision issues.
 *********************************************************************************************************
 */
int r_cmp(struct rational *a, struct rational *b);

/*
 *********************************************************************************************************
 *
 *                                          RATIONAL ABS
 *
 * Description: Computes the absolute value of a rational number.
 *
 * Arguments: a   The rational number to find the absolute value of.
 *
 * Returns: A rational number representing |a|.
 *
 * Notes: Takes the absolute value of both numerator and denominator.
 *********************************************************************************************************
 */
struct rational r_abs(struct rational a);