#ifdef __cplusplus
extern "C" {
#endif
#include "vlist.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

static int check_index(vlist vlist, long index) {
    return !(index >= 0 && index < vlist->size);
}

static long next_path_length(long size, long current_index, long target) {
    return target >= current_index ? target - current_index : size - current_index + target;
}

void* vlist_get(vlist this_vlist, long index) {
    if (check_index(this_vlist, index) != 0)
    {
        return NULL;
    }
    if (next_path_length(this_vlist->size, this_vlist->current_idx, index) < this_vlist->size / 2)
    {
        while (this_vlist->current_idx != index)
        {
            this_vlist->current = ((__VLIST_NODE_STRUCT_TYPE*)(this_vlist->current))->next;
            this_vlist->current_idx++;
            if (this_vlist->current_idx >= this_vlist->size)
            {
                this_vlist->current_idx = 0;
            }
        }
    }
    else
    {
        while (this_vlist->current_idx != index)
        {
            this_vlist->current = ((__VLIST_NODE_STRUCT_TYPE*)(this_vlist->current))->prev;
            this_vlist->current_idx--;
            if (this_vlist->current_idx < 0)
            {
                this_vlist->current_idx = this_vlist->size - 1;
            }
        }
    }
    return this_vlist->current;
}
const void* vlist_get_const(vlist this_vlist, long index) {
    return vlist_get(this_vlist, index);
}
int vlist_add(vlist this_vlist, const void* node) {
    void* node_copy = malloc(this_vlist->node_size);
    if (node_copy == NULL)
    {
        return VLIST_ERROR_MALLOC_FAIL;
    }
    memcpy(node_copy, node, this_vlist->node_size);
    vlist_quick_add(this_vlist, node_copy);
    return 0;
}
void vlist_quick_add(vlist this_vlist, void* node) {
    if (this_vlist->size == 0)
    {
        ((__VLIST_NODE_STRUCT_TYPE*)node)->next = node;
        ((__VLIST_NODE_STRUCT_TYPE*)node)->prev = node;
        this_vlist->current = node;
        this_vlist->current_idx = 0;
    }
    else {
        __VLIST_NODE_STRUCT_TYPE* iptr = this_vlist->get(this_vlist, this_vlist->size - 1);
        ((__VLIST_NODE_STRUCT_TYPE*)(iptr->next))->prev = node;
        ((__VLIST_NODE_STRUCT_TYPE*)node)->next = iptr->next;
        ((__VLIST_NODE_STRUCT_TYPE*)node)->prev = iptr;
        iptr->next = node;
    }
    this_vlist->size++;
}
int vlist_insert(vlist this_vlist, long index, const void* node) {
    if (check_index(this_vlist, index) != 0)
    {
        return VLIST_ERROR_INVALID_INDEX;
    }
    void* node_copy = malloc(this_vlist->node_size);
    if (node_copy == NULL)
    {
        return VLIST_ERROR_MALLOC_FAIL;
    }
    memcpy(node_copy, node, this_vlist->node_size);
    return vlist_quick_insert(this_vlist, index, node_copy);
}
int vlist_quick_insert(vlist this_vlist, long index, void* node) {
    if (check_index(this_vlist, index) != 0)
    {
        return VLIST_ERROR_INVALID_INDEX;
    }
    __VLIST_NODE_STRUCT_TYPE* iptr = this_vlist->get(this_vlist, index);
    ((__VLIST_NODE_STRUCT_TYPE*)(iptr->prev))->next = node;
    ((__VLIST_NODE_STRUCT_TYPE*)node)->prev = iptr->prev;
    ((__VLIST_NODE_STRUCT_TYPE*)node)->next = iptr;
    iptr->prev = node;
    this_vlist->size++;
    this_vlist->current_idx++;
    return 0;
}
int vlist_remove(vlist this_vlist, long index) {
    if (check_index(this_vlist, index) != 0)
    {
        return VLIST_ERROR_INVALID_INDEX;
    }
    __VLIST_NODE_STRUCT_TYPE* iptr = this_vlist->get(this_vlist, index);
    this_vlist->current = iptr->prev;
    ((__VLIST_NODE_STRUCT_TYPE*)(iptr->prev))->next = iptr->next;
    ((__VLIST_NODE_STRUCT_TYPE*)(iptr->next))->prev = iptr->prev;
    free(iptr); iptr = NULL;
    this_vlist->size--;
    if (this_vlist->size == 0)
    {
        this_vlist->current = NULL;
        this_vlist->current_idx = -1;
    }
    else {
        this_vlist->current_idx--;
        if (this_vlist->current_idx < 0)
        {
            this_vlist->current_idx = this_vlist->size - 1;
        }
    }
    return 0;
}
void vlist_foreach(vlist this_vlist, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra) {
    for (long i = 0; i < this_vlist->size; i++)
    {
        if (run(this_vlist, i, extra) != 0) {
            break;
        }
    }
}
void vlist_foreach_reverse(vlist this_vlist, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra) {
    for (long i = this_vlist->size - 1; i >= 0; i--)
    {
        if (run(this_vlist, i, extra) != 0) {
            break;
        }
    }
}
long vlist_flush(vlist this_vlist, VLIST_FILTER_FUNC_TYPE* filter, void* extra) {
    long flushed = 0;
    for (long i = this_vlist->size - 1; i >= 0; i--)
    {
        if (!filter(this_vlist, i, extra)) {
            this_vlist->remove(this_vlist, i);
            flushed++;
        }
    }
    return flushed;
}
void vlist_clear(vlist this_vlist) {
    while (this_vlist->size > 0)
    {
        this_vlist->remove(this_vlist, 0);
    }
}

vlist make_vlist(size_t node_size) {
    vlist res = malloc(sizeof(struct vlist_struct));

    if (res == NULL)
    {
        return NULL;
    }

    res->current = NULL;
    res->current_idx = -1;
    res->size = 0;

    res->node_size = node_size;

    res->get = vlist_get;
    res->get_const = vlist_get_const;
    res->add = vlist_add;
    res->quick_add = vlist_quick_add;
    res->insert = vlist_insert;
    res->quick_insert = vlist_quick_insert;
    res->remove = vlist_remove;
    res->foreach = vlist_foreach;
    res->foreach_reverse = vlist_foreach_reverse;
    res->flush = vlist_flush;
    res->clear = vlist_clear;

    return res;
}

void delete_vlist(vlist vlist_, vlist* vlist_ptr) {
    if (vlist_ == NULL)
    {
        return;
    }
    while (vlist_->size > 0)
    {
        vlist_->remove(vlist_, 0);
    }
    free(vlist_);
    *vlist_ptr = NULL;
}

#ifdef __cplusplus
}
#endif
