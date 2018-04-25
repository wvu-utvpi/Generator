/*
 * halfint.h
 * 
 * half_int is an exact representation of a half-integral number, generated by shifting an integral number one bit to the left.
 * The half_int is especially useful for representing integral numbers after they've been divided by two, by not actually
 * modifying the original integral representation in any way, but treating the least significant bit as the halves place.
 *
 * The underlying integer representation will properly handle addition, subtraction, and comparison of half_ints. It will not,
 * however, properly handle multiplication and division. Thus, these operations are not supported for half_ints.
 */

#ifndef _HALFINT_H
#define _HALFINT_H

#include <stdbool.h>

/*
 * To my knowledge, no C compiler has the option to warn when a typedef'd primitive data type is used in combination with non-
 * typedef'd (or differently typedef'd) primitive data types. The user must take care that half_ints are only added to, subtracted
 * from, or compared against other half_ints. Cast ints to half_ints with the intToHalfInt() function, in order to facilitate 
 * these operations with mixed types.
 */
typedef int half_int;

/*
 * intToHalfInt()
 *   Returns an exact half_int representation of the input int
 */
half_int intToHalfInt(int input);

/*
 * halfIntToInt()
 *   Returns an int representation of the input half_int. Handles fractional half_ints the same way casting a floating-point value
 *   to an int works, rounding toward zero.
 */
int halfIntToInt(half_int input);

/*
 * halfIntToDouble()
 *   Returns a double representation of the input half_int. Considering floating point arithmetic inaccuracy, mostly useful for 
 *   outputing to printing functions.
 */
double halfIntToDouble(half_int input);

/*
 * intDivBy2ToHalfInt()
 *   Returns the half_int representation of the input int, after it has been divided by 2. This makes no change to the underlying
 *   integer representation, but its use is recommended for the sake of clarity.
 */
half_int intDivBy2ToHalfInt(int input);

/*
 * halfIntIsIntegral()
 *   Returns true if the input half_int represents a whole integer, false otherwise.
 */
bool halfIntIsIntegral(half_int input);

/*
 * halfIntCeil()
 *   Returns the half_int representation of the ceiling of the input half_int.
 */
half_int halfIntCeil(half_int input);

/*
 * halfIntFloor()
 *   Returns the half_int representation of the floor of the input half_int.
 */
half_int halfIntFloor(half_int input);

#endif
