/*
 * Roman Hargrave. 2015, Apache License w/ reserved privileges for author when stated.
 * Created for use in CSCE 1040 project at UNT.
 *
 * roman@hargrave.info.
 *
 * Not published.
 *
 * It's a list... It's a deque.. It's GBLinkedList!
 *
 * Provides a doubly linked collection with deque-like features. Each value inserted is
 * encapsulated in a GBLinkedNode (which should not be used externally), which is memory
 * managed internally by the linked list library.
 *
 * Such internal memory management is accomplished by the use of a reference broker pattern.
 * When allocating a list, a new ``head`` is created, which on the outward facing side has an untyped
 * pointer. This ``head'' is aliased internally, and is updated when appropriate. This prevents client code
 * that uses the list from holding on to potentially dead references to list parts, as it impossible to interact
 * directly with them (all references go through the API, which insures all is done correctly).
 */

#ifndef _H_LINKED_LIST_GB
    #define _H_LINKED_LIST_GB
    #include <stddef.h>
    #include <stdbool.h>
    #include "../util.h"

// -- List API Types ---------------------------------------------------------------------------------------------------

typedef int (*GBLL_Comparator)(const void* a, const void* b);
typedef size_t GBListSize;

// -- Frontend Objects -------------------------------------------------------------------------------------------------

/*
 * API Facing structure for reference broker model
 */
typedef struct S_GBList {

    const GBListSize size;

} GBList;

typedef struct S_GBList_Iterator {

    /*
     * Current data
     */
    void* data;

} GBIterator;

// -- Helper Macros ----------------------------------------------------------------------------------------------------

/*
 * Handy-dandy preprocessor macro that iterates through a list and handles the boiler plate!
 * This borders on preprocessor abuse (don't flog me!)
 */
#define GBList_with(head, with, lambda) {\
    GBIterator* iter = GBList_iterator(head);\
    do {\
        void* with = GBIterator_next(iter);\
        (lambda);\
    } while ( GBIterator_hasNext(iter) );\
    GBIterator_destroy(iter);\
}

/*
 * Preprocessor macro that allows you to perform a Map function with `lambda` where lambda is an expression.
 * `x` will define the name of the variable visible inside the lambda which represents the current value.
 *
 * There is no question is to whether this is preprocessor abuse.
 * It is __flagrant__ preprocessor abuse.
 *
 * It also uses GCC compound statements.
 */
#define GBList_mapWith(head, x, lambda) ({\
    GBList* list = GBList_new();\
    GBList_with(head, currentGBList, {\
        void* x = GBIterator_next(iter);\
        void* yield = (void*) ( lambda );\
        GBList_append(list, yield);\
    });\
    list;\
})

#define GBList_foldWith(head, initial, x, y, lambda) ({\
    typeof(initial) x = initial;\
    GBList_with(head, _node, {\
        typeof(initial) y = (typeof(initial)) _node->data;\
        x = (lambda);\
    })\
    x;\
})

// -- Iterator API -----------------------------------------------------------------------------------------------------

// ---- Management -----------------------------------------------------------------------------------------------------

$NotNull()
GBIterator* GBList_iterator(GBList*);

$NotNull()
GBIterator* GBList_tailIterator(GBList*);

$NotNull()
GBIterator* GBList_loopIterator(GBList*);

$NotNull()
void GBIterator_destroy(GBIterator*);

// ---- Access ---------------------------------------------------------------------------------------------------------

$NotNull()
bool GBIterator_hasNext(GBIterator*);

$NotNull()
void* GBIterator_next(GBIterator*);

$NotNull()
bool GBIterator_hasPrior(GBIterator*);

$NotNull()
void* GBIterator_prior(GBIterator*);

// -- List API ---------------------------------------------------------------------------------------------------------

// ---- Constructors ---------------------------------------------------------------------------------------------------

/*
 * Initialize a new GBList
 * To allocate with an initial value, see GBLinkedList_new_with()
 */
$Allocator
GBList* GBList_new();

$Allocator
GBList* GBList_new_with(void* data);

// ---- Destructors ----------------------------------------------------------------------------------------------------

/*
 * Free a GBList (note that this will not free #data)
 */
$NotNull(1)
void GBList_free(GBList* list) ;

// ---- Inspection -----------------------------------------------------------------------------------------------------

/*
 * Determines the size of the list by counting the number of nodes in
 * the list
 */
$NotNull(1)
size_t GBList_size(GBList* list);

/*
 * Returns the value at the top of the list
 */
$NotNull()
void* GBList_first(GBList* list);

/*
 * Returns the value at the end of the list
 */
$NotNull()
void* GBList_last(GBList* list);

/*
 * Counts nodes up to index and returns the value of the node at that point.
 * Will return NULL if index exceeds the number of nodes in the chain
 */
$NotNull(1)
void* GBList_get(GBList* list, size_t index);

/*
 * Performs GBLinkedList_findGBList and then returns the value of that node.
 * Returns NULL if no value is found
 */
$NotNull(1)
GBList* GBList_find(GBList*, const void*, GBLL_Comparator);

/*
 * Returns true if GBLinkedList_findGBList(...) exists, and (...)->value is non-NULL
 */
$NotNull(1, 3)
bool GBList_contains(GBList*, const void*, GBLL_Comparator);

// ---- Manipulation ---------------------------------------------------------------------------------------------------

// ------ Additive -----------------------------------------------------------------------------------------------------

/*
 * Puts `ptr` at the top of the list
 */
$NotNull(1)
void GBList_prepend(GBList* list, void* ptr);

/*
 * Inserts in to the position in the list where the comparator says that this should go
 */
$NotNull(1)
void GBList_append(GBList* list, void* ptr);

/*
 * Like `_insert`, however, instead of replacing the preexisting node, a new node will be initialized
 * and 'spliced' between index->preceding and index itself, thus meaning that index->preceding is the
 * node containing `ptr`.
 *
 * This additionally means that index is shifted right by one index
 *
 * Warning: I am O(k) where `k` is the number of nodes (!)
 */
$NotNull(1)
bool GBList_insertBefore(GBList* list, void* ptr, size_t index);

$NotNull(1)
bool GBList_insertAfter(GBList* list, void* ptr, size_t index);

// ------ Neutral ------------------------------------------------------------------------------------------------------

/*
 * Counts nodes up to `index`, returning false if the chain ends before `index` is reached.
 * Once `index` is reached, the node at the position will have its value updated to
 * `ptr` instead of its previous value, after which a value of true will be returned.
 *
 * You shouldn't use this on large collections
 *
 * Warning: I am O(k) where `k` is the number of nodes (!)
 */
$NotNull(1)
bool GBList_set(GBList* list, void* ptr, size_t index);

$NotNull()
void GBList_reverse(GBList* list);

// ------ Subtractive --------------------------------------------------------------------------------------------------

$NotNull()
void* GBList_pop(GBList*);

/*
 * Pop from the tail and return the value that was previously at the tail.
 * Returns NULL if the list is 0 nodes long, or that element should have a NULL value.
 */
$NotNull()
void* GBList_eject(GBList*);

/*
 * Performs the following operations:
 *  - GBLinkedList_findGBList(list, key)
 *  - NULL? Return false
 *  - GBLinkedList_unlink(node)
 *  - free(node)
 *  - Return true
 */
$NotNull(1, 3)
GBListSize GBList_remove(GBList*, const void*, GBLL_Comparator);

$NotNull(1)
bool GBList_removeAt(GBList*, GBListSize);

$NotNull()
void GBList_clear(GBList*);

// ------ Computations -------------------------------------------------------------------------------------------------

/*
 * Performs an operation on each element in the list taking two arguments:
 *
 *      (x, y) => f(x,y)
 *
 *      where
 *          for the first iteration
 *              x is provided to the function call
 *          for all other iterations
 *              x is the result of the previous operation
 *          y is the nth element in the list
 *
 *  A an example use case might be summing all values in a list:
 *
 *      let Sum(x, y) in
 *          x + y
 *
 *      FoldRight(Sequence(1, 2, 3), 0, sum) ==
 *
 *          ( 0 + 1 -> 1
 *            1 + 2 -> 3
 *            3 + 3 -> 6 )
 *
 *          == 6
 *
 *
 *      Essentially, this can be unwound as follows:
 *
 *          # Where `N` is the nth index of Sequence(1, 2, 3)
 *
 *          let x 0
 *
 *          # N = 0, x = 0
 *          let x Sum(x, 1)
 *
 *          # N = 1, x = 1
 *          let x Sum(x, 2)
 *
 *          # N = 2, x = 3
 *          let x Sum(x, 3)
 *
 *          x = 6
 *
 *      Alternatively,
 *
 *          (sum 3
 *              (sum 2
 *                  (sum 1 0)))
 *
 *      Which simplifies to:
 *
 *          (sum 3 2 1)
 *
 *          ;; or
 *          (sum 1 2 3)
 *
 */
$NotNull(1, 3)
void* GBList_foldRight(GBList*, const void*, void*(*)(const void*, const void*));

/*
 * Maps the output of a function applied to element N in a list
 *
 * Example:
 *
 *      let Product(x)(y) in
 *          x * y
 *
 *      Map(Sequence(1, 2, 3), Product(2)) = Sequence(2, 4, 6)
 *
 *  Rules:
 *
 *      # Where Application(x) is an arbitrary function that is both pure and deterministic
 *      # such that Application(x) = 'x , and Inverse(Application)('x) = x are both true at any given time
 *      let Application(x)
 *
 *      let a Sequence(1, 2, 3)
 *      let b Map(a, Application)
 *
 *      # Where N is an arbitrary index within both `a` and `b`
 *      Take(b, N) = Application(Take(a, N))
 *
 */
$NotNull()
GBList* GBList_map(GBList*, void*(*)(const void*));

// ------ Sort ---------------------------------------------------------------------------------------------------------

/*
 * Sort the specified list using the internal comparator
 */
$NotNull()
void GBList_sort(GBList*, GBLL_Comparator);

#endif