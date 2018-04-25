/*
 * constraint.h
 *
 * A standard representation of UTVPI constraints, portable across inequality system solvers, input file parsers, and anything
 * else. It is expected that inequality system solvers will translate Constraints into their own internal representations and can
 * translate their own internal representations back into Constraints for returning proofs of infeasibility, or for any other
 * purpose.
 *
 * Also includes a basic list structure and associated functionality to store multiple Constraints.
 */

#ifndef _CONSTRAINT_H
#define _CONSTRAINT_H

#include <stdio.h>
#include "voidRefList.h"

#define VARIABLES_PER_CONSTRAINT 2

/*
 * CONSTRAINT_PLUS and CONSTRAINT_MINUS represent positive and negative variable signs. CONSTRAINT_NONE represents that a variable
 * is not present, i.e. that the other variable within the Constraint struct is the only variable present within an absolute
 * constraint.
 */
typedef enum ConstraintSign {
  CONSTRAINT_PLUS,
  CONSTRAINT_MINUS,
  CONSTRAINT_NONE,
} ConstraintSign;

typedef struct Constraint Constraint;
typedef struct VoidRefLIst ConstraintRefList;

/*
 * sign[i] and index[i] represent all information about variable i, and weight gives the weight of the constraint. By convention,
 * absolute constraints, consisting of only a single variable and a weight, have sign[1] == CONSTRAINT_NONE, and index[1] == 0.
 * The single variable should be represented by sign[0] and index[0].
 */
struct Constraint {
  ConstraintSign sign[VARIABLES_PER_CONSTRAINT];
  int index[VARIABLES_PER_CONSTRAINT];
  int weight;
};

/*
 * fputConstraint() prints the constraint equation corresponding to constraint to output
 *
 * constraint - pointer to a Constraint struct to print as a constraint equation
 * output - FILE pointer to print the constraint equation to
 */
void fputConstraint(Constraint * constraint, FILE * output);

/*
 * generateConstraintRefList() allocates and initializes a ConstraintRefList, returning a pointer to it.
 */
ConstraintRefList * generateConstraintRefList();

/*
 * constraintRefListAppend() adds a Constraint pointer to the end of a ConstraintRefList.
 *
 * crl - pointer to a ConstraintRefList to append a Constraint pointer to
 * constraint - pointer to a constraint, which will be added to the end of crl
 */
void constraintRefListAppend(ConstraintRefList * crl, Constraint * constraint);

/*
 * constraintRefListPrepend() adds a constraint pointer to the beginning of a ConstraintRefList.
 *
 * crl - pointer to a ConstraintRefList to prepend with a Constraint pointer
 * constraint - pointer to a Constraint, which will be added to the beginning of crl
 */
void constraintRefListPrepend(ConstraintRefList * crl, Constraint * constraint);

/*
 * constraintRefListNext() allows for the ConstraintRefList to be iterated through. Each call to this function returns another
 * element within the ConstraintRefList, until the end of the list is reached, when NULL is returned. New elements should not be
 * added to the ConstraintRefList while this process is ongoing. Reaching the end of the list resets the internal iterator which
 * enables this process. If iteration through the list ends before the list has been fully traversed,
 * constraintRefListIteratorReset() should be called.
 *
 * crl - the ConstraintRefLIst to be iterated through
 */
Constraint * constraintRefListNext(ConstraintRefList * crl);

/*
 * constraintRefListIteratorReset() resets the ConstraintRefList's internal iterator after a partial traverse of the
 * ConstraintRefLIst has been conducted using constraintRefListNext().
 *
 * crl - the ConstraintRefList
 */
void constraintRefListIteratorReset(ConstraintRefList * crl);

/*
 * freeConstraintRefList() frees the input constraintRefList, as well as referenced Constraints.
 *
 * crl - pointer to the ConstraintRefList to be freed
 */
void freeConstraintRefList(ConstraintRefList * crl);

#endif
