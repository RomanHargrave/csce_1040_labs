#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "linkedlist.h"

#define lreal(list) ((GBListReal*)list)
#define ireal(iter) ((GBIteratorReal*)iter)

// Internal List Data Structures ---------------------------------------------------------------------------------------

/*
 * A node in a list.
 * On their own, nodes form the entire list.
 * This is referred to throughout the code in this file as a `chain'
 */
typedef struct S_GBList_Node {

    struct S_GBList_Node* prior;

    struct S_GBList_Node* next;

    void* data;

} Node;

/*
 * Real list broker.
 * This model exists for each list.
 * It holds a size, which is updated by each function which modifies the number of links. The size is available in client code
 * It also holds references to both the head and tail, allowing for O(1) prepend, append, eject, and pop.
 *
 * Due to C's memory model, it can be cast to GBList, which will work because GBList's only element, size, is in the same
 * place as cachedSize, and is also of the same type. This is used to the advantage of the list library, because it makes
 * corrupting the node order from client code impossible without intentionally doing so.
 */
typedef struct S_GBList_Real {

    size_t size;

    Node* head;

    Node* tail;

} GBListReal;

/*
 * Real Iterator.
 * Created by GBList_iterator and destroyed by GBIterator_destroy.
 *
 * It holds a void pointer that represents the data in the current node, and which is available to client code.
 *
 * In the internal implementation, it holds a pointer to the last node passed over, and the next node.
 * By default, it is meant to do forward iteration, but can be initialized for backward iteration using GBList_tailIterator
 * or for circular iteration using GBList_loopIterator.
 */
typedef struct S_GBList_Iterator_Real {

    void* data;

    Node* prior;
    Node* next;

} GBIteratorReal;

// Internal helpers ----------------------------------------------------------------------------------------------------

// -- Node Helpers -----------------------------------------------------------------------------------------------------

/*
 * Recursively traverse to the last node in a chain
 */
Node* Node_tail(Node* head) {
    unless(head) return NULL;
    unless(head->next) return head;
    return Node_tail(head->next);
}

Node* Node_head(Node* tail) {
    unless(tail) return NULL;
    unless(tail->prior) return tail;
    return Node_head(tail->prior);
}

size_t Node_countLinks(Node* head) {
    unless(head) return 0;
    return 1 + Node_countLinks(head->next);
}

bool Node_eq(Node* a, Node* b) {
    return (a->prior == b->prior)
           && (a->data == b->data)
           && (a->next == b->next);
}

// -- List Helpers -----------------------------------------------------------------------------------------------------

/*
 * Traverse to the Ith node in a given GBListReal.
 * In attempt to optimize traversal time (such that it may be O(N/2) as opposed to O(N)),
 * the center of the list, P, will be calculated by calculating half of size, and determining weather
 *  I lies before or after that pivot.
 * If I lies before the pivot, the node chain will be traversed from the first element in the chain until I is reached
 * If I lies after the pivot, the node chain will be traversed backwards from the last element in the chain until
 *  I is reached.
 *
 *
 *  If  I = P then backward reversal will be the default mode of traversal.
 */
Node* List_nthNode(GBListReal* list, size_t index) {

    if (list->size < 2) return list->head;
    if (index > list->size) return NULL;

    size_t pivot = list->size / 2;

    /*
     * If index is after the pivot value, traverse up, otherwise down
     */
    Node* current = NULL;
    if (index >= pivot) {
        current = list->tail;
        for (size_t idx = list->size - 1;
             idx > index; --idx) {
            current = current->prior;
        }
    } else {
        current = list->head;
        for (size_t idx = 0; idx < index; ++idx) current = current->next;
    }

    return current;
}

$WrapperFunction
void List_onListUpdate(GBListReal* list) {
    if(!list->head && list->tail) {
        list->head = Node_head(list->tail);
    } else if(!list->tail && list->head) {
        list->tail = Node_tail(list->head);
    } else if(!list->head && !list->head) {
        list->size = 0;
    }

    if(list->size < 1 && list->head) {
        list->size = Node_countLinks(list->head);
    }
}

/*
 * Allocated a new Node
 */
$Allocator $WrapperFunction
Node* Node_allocate(void* data) {
    Node* node = gb_new(Node);
    node->data = data;
    return node;
}

/*
 * Perform necessary procedures to dispose of a node
 */
$WrapperFunction
void Node_destroy(Node* node) {
    gb_free(node);
}

$Optimize(2)
void Node_destroyChain(Node* head) {

    unless(head) return;

    Node* next = head->next;

    head->next = NULL;
    head->prior = NULL;
    Node_destroy(head);

    Node_destroyChain(next);
}

// ---------------------------------------------------------------------------------------------------------------------

// -- Iterator API -----------------------------------------------------------------------------------------------------

// ---- Iterator Management --------------------------------------------------------------------------------------------

GBIterator* GBList_iterator(GBList* list) {
    unless(list->size > 0) return NULL;

    GBIteratorReal* iterator = gb_new(GBIteratorReal);

    iterator->data = NULL;
    iterator->next = lreal(list)->head;

    return (GBIterator*) iterator;
}

GBIterator* GBList_tailIterator(GBList* list) {
    unless(list->size > 0) return NULL;

    GBIteratorReal* iterator = gb_new(GBIteratorReal);

    iterator->data = NULL;
    iterator->prior = lreal(list)->tail;

    return (GBIterator*) iterator;
}

GBIterator* GBList_loopIterator(GBList* list) {
    GBIteratorReal* iterator = ireal(GBList_iterator(list));
    unless(iterator) return NULL;

    iterator->prior = lreal(list)->tail;

    return (GBIterator*) iterator;
}


void GBIterator_destroy(GBIterator* iterator) {
    gb_free(ireal(iterator));
}

// ---- Iterator Access ------------------------------------------------------------------------------------------------

$WrapperFunction
bool GBIterator_hasNext(GBIterator* iterator) {
    return ireal(iterator)->next != NULL;
}

void* GBIterator_next(GBIterator* iterator) {
    unless(GBIterator_hasNext(iterator)) return NULL;

    GBIteratorReal* real = ireal(iterator);

    real->data = real->next->data;

    Node* next = real->next->next;
    real->prior = real->next;
    real->next = next;

    return real->data;
}

bool GBIterator_hasPrior(GBIterator* iterator) {
    return ireal(iterator)->prior != NULL;
}

void* GBIterator_prior(GBIterator* iterator) {
    unless(GBIterator_hasPrior(iterator)) return NULL;

    GBIteratorReal* real = ireal(iterator);

    real->data = real->prior->data;

    Node* next = real->prior->prior;
    real->next = real->prior;
    real->prior = next;

    return real->data;
}

// -- List API ---------------------------------------------------------------------------------------------------------

// ---- Constructors / Factories ---------------------------------------------------------------------------------------

GBList* GBList_new() {

    GBListReal* listBroker = gb_new(GBListReal);
    listBroker->head = NULL;
    listBroker->tail = NULL;
    listBroker->size = 0;

    return (GBList*) listBroker;
}

$WrapperFunction
GBList* GBList_new_with(void* initialData) {
    GBList* list = GBList_new();
    GBList_append(list, initialData);
    return list;
}

// ---- Destructors ----------------------------------------------------------------------------------------------------

void GBList_free(GBList* list) {
    Node_destroyChain(lreal(list)->head);
    gb_free(lreal(list));
}

// ---- List Inspection ------------------------------------------------------------------------------------------------

$WrapperFunction
GBListSize GBList_size(GBList* list) {
    return list->size;
}

$WrapperFunction
void* GBList_first(GBList* list) {
    unless(lreal(list)->head) return NULL;
    return ((GBListReal*) list)->head->data;
}

$WrapperFunction
void* GBList_last(GBList* list) {
    unless(lreal(list)->tail) return NULL;
    return ((GBListReal*) list)->tail->data;
}

$WrapperFunction
void* GBList_get(GBList* list, size_t index) {
    Node* node = List_nthNode(lreal(list), index);
    unless(node) return NULL;
    return node->data;
}

GBList* GBList_find(GBList* list, const void* key, GBLL_Comparator comparator) {

    GBList* found = GBList_new();

    GBList_with(list, current, {
        if (comparator(key, current) == 0) GBList_append(found, current);
    });

    return found;
}

bool GBList_contains(GBList* list, const void* key, GBLL_Comparator comparator) {

    bool wasFound = false;

    GBList_with(list, value, {
        if(comparator(key, value) == 0){
            wasFound = true;
            break;
        }
    })

    return wasFound;
}

// ---- List Manipulation ----------------------------------------------------------------------------------------------

// ------ Addition / Insertion -----------------------------------------------------------------------------------------

void GBList_prepend(GBList* list, void* ptr) {

    GBListReal* real = lreal(list);

    Node* newHead = Node_allocate(ptr);

    newHead->next = real->head;

    if(real->head) real->head->prior = newHead;

    real->head = newHead;

    ++real->size;

    List_onListUpdate(real);
}

void GBList_append(GBList* list, void* ptr) {

    GBListReal* real = lreal(list);

    Node* tail = real->tail;
    Node* newTail = Node_allocate(ptr);

    real->tail = newTail;
    real->tail->prior = tail;

    if(tail) tail->next = real->tail;

    ++real->size;

    List_onListUpdate(real);
}

bool GBList_insertBefore(GBList* list, void* ptr, size_t index) {

    GBListReal* real = lreal(list);
    Node* at = List_nthNode(real, index);

    unless(at) return false;

    if (at == real->head) {
        GBList_prepend(list, ptr);
    } else {

        Node* new = Node_allocate(ptr);

        // [1]=[2]=[3]
        Node* oldPrior = at->prior;

        // [1] [4]=[2]=[3]
        at->prior = new;
        new->next = at;

        // [1]=[4]=[2]=[3]
        if (oldPrior) {
            new->prior = oldPrior;
            oldPrior->next = new;
        }

        ++real->size;
    }

    return true;
}

bool GBList_insertAfter(GBList* list, void* ptr, size_t index) {

    GBListReal* real = lreal(list);
    Node* at = List_nthNode(real, index);

    unless(at) return false;

    if (at == real->tail) {
        GBList_append(list, ptr);
    } else {

        Node* new = Node_allocate(ptr);

        // [1]=[2]=[3]
        Node* oldNext = at->next;

        // [1]=[2]=[4] [3]
        at->next = new;
        new->prior = at;

        // [1]=[2]=[4]=[3]
        if (oldNext) {
            new->next = oldNext;
            oldNext->prior = new;
        }

        ++real->size;
    }

    return true;
}

bool GBList_set(GBList* list, void* ptr, size_t index) {

    Node* targetNode = List_nthNode(lreal(list), index);

    unless(targetNode) return false;

    targetNode->data = ptr;

    return true;
}

// ------ Neutral Modification -----------------------------------------------------------------------------------------

void GBList_reverse(GBList* list) {

    GBListReal* real = lreal(list);

    Node* current = real->head;
    Node* swap;

    do {
        swap = current->prior;
        current->prior = current->next;
        current->next = swap;
        current = current->prior;
    } while (current);

    Node* head = real->head;
    real->head = real->tail;
    real->tail = head;
}

// ------ Negative Modification ----------------------------------------------------------------------------------------

void* GBList_pop(GBList* list) {

    GBListReal* real = lreal(list);

    unless(real->head && real->size > 0) return NULL;

    // [<]=[_]=[_]=[>]
    Node* oldHead = real->head;

    // [_]-[<]=[_]=[>]
    real->head = oldHead->next;

    --real->size;

    // [_] [<]=[_]=[>]
    real->head->prior = NULL;

    void* oldData = oldHead->data;

    // [<]=[_]=[>]
    Node_destroy(oldHead);

    return oldData;
}

void* GBList_eject(GBList* list) {

    GBListReal* real = lreal(list);

    unless(real->tail && real->size > 0) return NULL;

    // [<]=[_]=[_]=[>]

    Node* oldTail = real->tail;

    // [<]=[_]=[>]-[_]
    real->tail = oldTail->prior;

    --real->size;

    // [<]=[_]=[>] [_]
    real->tail->next = NULL;

    void* oldData = oldTail->data;

    // [<]=[_]=[>]
    Node_destroy(oldTail);

    return oldData;
}

GBListSize GBList_remove(GBList* list, void const* pVoid, GBLL_Comparator comparator) {

    GBListReal* real = lreal(list);
    GBListSize removalCount = 0;

    Node* current = real->head;
    while (current) {
        if (comparator(pVoid, current->data) == 0) {
            if(current->prior) current->prior->next = current->next;
            if(current->next) current->next->prior = current->prior;
            Node* old = current;
            current = current->next;
            Node_destroy(old);
            ++removalCount;
        } else {
            current = current->next;
        }
    }

    real->size -= removalCount;

    return removalCount;
}

bool GBList_removeAt(GBList* list, GBListSize index) {

    GBListReal* real = lreal(list);
    Node* at = List_nthNode(real, index);

    unless(at) return false;

    at->prior->next = at->next;
    at->next->prior = at->prior;

    Node_destroy(at);

    --real->size;

    return true;
}

$WrapperFunction
void GBList_clear(GBList* list) {
    d_printf("%s(<GBList:0x%X>\n", __FUNCTION__, list);
    Node_destroyChain(lreal(list)->head);
    lreal(list)->head = NULL;
    lreal(list)->tail = NULL;
    lreal(list)->size = 0;
}

// ------ List Computations --------------------------------------------------------------------------------------------

void* GBList_foldRight(GBList* list, const void* x, void* (* lambda)(const void*, const void*)) {

    GBIterator* iterator = GBList_iterator(list);

    void* accum = NULL;
    if (GBIterator_hasNext(iterator)) {
        accum = lambda(x, GBIterator_next(iterator));
        while (GBIterator_hasNext(iterator)) {

            void* lastAccum = accum;

            accum = lambda(accum, GBIterator_next(iterator));

            gb_free(lastAccum);

        }
    }

    GBIterator_destroy(iterator);

    return accum;
}

GBList* GBList_map(GBList* list, void* (* lambda)(const void*)) {
    return GBList_mapWith(list, x, lambda(x));
}

// ------ List Sorting -------------------------------------------------------------------------------------------------

// -------- List Sort Helpers ------------------------------------------------------------------------------------------

$NotNull()
void Node_appendChains(Node* appendTo, Node* appendFrom) {

    d_printf("| |- %s(<Node:0x%X>, <Node:0x%X>)\n", __FUNCTION__, appendTo, appendFrom);

    Node* current = appendFrom;

    Node* tail = Node_tail(appendTo);
    tail->next = Node_allocate(NULL);
    tail = tail->next;

    Node* last = NULL;

    do {

        tail->data = current->data;

        if (last) tail->prior = last;

        current = current->next;

        if (current) {
            last = tail;
            tail = Node_allocate(NULL);
            last->next = tail;
        }

    } while (current);

}

$NotNull(1, 2)
Node* List_sortAndMergeChains(Node* a, Node* b, GBLL_Comparator comparator) {

    d_printf("|- %s(<Node:0x%X>, <Node:0x%X>)\n", __FUNCTION__, a, b);

    unless(a && b) return NULL;

    Node* result = Node_allocate(NULL);

    Node* tail = result;
    Node* last = NULL;

    do {

        int diff = comparator(a->data, b->data);

        if (diff <= 0) {
            tail->data = a->data;
            a = a->next;
        } else {
            tail->data = b->data;
            b = b->next;
        }

        // Only prepare for the next append operation if there is more data
        if (a && b) {

            // assign last processed as tail
            last = tail;

            // create new tail
            tail = Node_allocate(NULL);

            // set the previous tail as being proceeded by the new tail
            last->next = tail;

            // Set the previous element of the new tail as being the previous tail
            tail->prior = last;
        }
    } while (a && b);

   // If one of the lists was longer than the other, append the remainder to the result list
    if (a) {
        d_printf("| |- result append a\n");
        Node_appendChains(result, a);
    }

    if (b) {
        d_printf("| |- result append b\n");
        Node_appendChains(result, b);
    }

    return result;
}

$NotNull(1, 3)
void List_sortImpl(GBListReal* list, size_t length, GBLL_Comparator comparator) {

    size_t pivot = length / 2;
    d_printf("%s(<Node:%X>, %lu) <Pivot->%lu>\n", __FUNCTION__, list, length, pivot);

    unless(length > 1 && list) return;

    GBListReal* left    = lreal(GBList_new());
    GBListReal* right   = lreal(GBList_new());

    {
        Node* current = list->head;

        // Populate left division
        for (size_t idx = 0; idx < (pivot - 1); ++idx, current = current->next) GBList_append(left, current->data);

        // Populate right division
        for (current; current; current = current->next) GBList_append(right, current->data);
    }

    {
        // Recursive sort down to minimum
        List_sortImpl(left, left->size, comparator);
        List_sortImpl(right, right->size, comparator);
    }

    Node* result;
    {
        bool invertMerge = (comparator(GBList_first(left), GBList_first(right)) <= 0);

        // Determine merge order
        if (invertMerge) {
            result = List_sortAndMergeChains(right->head, left->head, comparator);
        } else {
            result = List_sortAndMergeChains(left->head, right->head, comparator);
        }
    }

    {
        // Free left and right temp lists
        GBList_free(left);
        GBList_free(right);


        GBList_clear(list);
        list->head = result;
        list->tail = NULL;
        List_onListUpdate(list);
    }
}

// -------- List Sort API Implementation -------------------------------------------------------------------------------

$Weak
void GBList_sort(GBList* list, GBLL_Comparator comparator) {
    List_sortImpl(lreal(list), list->size, comparator);
}

// ---------------------------------------------------------------------------------------------------------------------

