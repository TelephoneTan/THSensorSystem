#ifndef VLIST
#define VLIST

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define __VLIST_NODE_STRUCT_TYPE struct{void*prev;void*next;}
#define VLISTNODE __VLIST_NODE_STRUCT_TYPE ;

#define VLIST_ERROR_INVALID_INDEX -1
#define VLIST_ERROR_MALLOC_FAIL -2

typedef struct vlist_struct* vlist;

// return non-zero to break
typedef int VLIST_RUNNABLE_FUNC_TYPE(vlist this_vlist, long i, void *extra);

// return zero to remove current node from vlist
typedef int VLIST_FILTER_FUNC_TYPE(vlist this_vlist, long i, void *extra);

typedef void* VLIST_GET_FUNC_TYPE(vlist this_vlist, long index);
typedef const void* VLIST_GET_CONST_FUNC_TYPE(vlist this_vlist, long index);
typedef int VLIST_ADD_FUNC_TYPE(vlist this_vlist, const void* node);
typedef void VLIST_QUICK_ADD_FUNC_TYPE(vlist this_vlist, void* node);
typedef int VLIST_INSERT_FUNC_TYPE(vlist this_vlist, long index, const void* node);
typedef int VLIST_QUICK_INSERT_FUNC_TYPE(vlist this_vlist, long index, void* node);
typedef int VLIST_REMOVE_FUNC_TYPE(vlist this_vlist, long index);
typedef void VLIST_FOREACH_FUNC_TYPE(vlist this_vlist, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra);
typedef void VLIST_FOREACH_REVERSE_FUNC_TYPE(vlist this_vlist, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra);
typedef long VLIST_FLUSH_FUNC_TYPE(vlist this_vlist, VLIST_FILTER_FUNC_TYPE* filter, void* extra);
typedef void VLIST_CLEAR_FUNC_TYPE(vlist this_vlist);

struct vlist_struct
{
    void* current;
    long current_idx;
    long size;
    size_t node_size;

    VLIST_GET_FUNC_TYPE* get;
    VLIST_GET_CONST_FUNC_TYPE* get_const;
    VLIST_ADD_FUNC_TYPE* add;
    VLIST_QUICK_ADD_FUNC_TYPE* quick_add;
    VLIST_INSERT_FUNC_TYPE* insert;
    VLIST_QUICK_INSERT_FUNC_TYPE* quick_insert;
    VLIST_REMOVE_FUNC_TYPE* remove;
    VLIST_FOREACH_FUNC_TYPE* foreach;
    VLIST_FOREACH_REVERSE_FUNC_TYPE* foreach_reverse;
    VLIST_FLUSH_FUNC_TYPE* flush;
    VLIST_CLEAR_FUNC_TYPE* clear;
};

vlist make_vlist(size_t node_size);
void delete_vlist(vlist vlist_, vlist* vlist_ptr);

void* vlist_get(vlist this_vlist, long index);
const void* vlist_get_const(vlist this_vlist, long index);
int vlist_add(vlist this_vlist, const void* node);
void vlist_quick_add(vlist this_vlist, void* node);
int vlist_insert(vlist this_vlist, long index, const void* node);
int vlist_quick_insert(vlist this_vlist, long index, void* node);
int vlist_remove(vlist this_vlist, long index);
void vlist_foreach(vlist this_vlist, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra);
void vlist_foreach_reverse(vlist this_vlist, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra);
long vlist_flush(vlist this_vlist, VLIST_FILTER_FUNC_TYPE* filter, void* extra);
void vlist_clear(vlist this_vlist);

#ifdef __cplusplus
}
#endif

#endif // VLIST