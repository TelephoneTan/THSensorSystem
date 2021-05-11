#include "vlist.h"

#include <stdlib.h>
#include <string.h>

static int check_index(vlist vlist, long index) {
    return !(index >= 0 && index < vlist->size);
}

static long next_path_length(long size, long current_index, long target) {
    return target >= current_index ? target - current_index : size - current_index + target;
}

void* vlist_get(vlist this, long index) {
    if (check_index(this, index) != 0)
    {
        return NULL;
    }
    if (next_path_length(this->size, this->current_idx, index) < this->size / 2)
    {
        while (this->current_idx != index)
        {
            this->current = ((__VLIST_NODE_STRUCT_TYPE*)(this->current))->next;
            this->current_idx++;
            if (this->current_idx >= this->size)
            {
                this->current_idx = 0;
            }
        }
    }
    else
    {
        while (this->current_idx != index)
        {
            this->current = ((__VLIST_NODE_STRUCT_TYPE*)(this->current))->prev;
            this->current_idx--;
            if (this->current_idx < 0)
            {
                this->current_idx = this->size - 1;
            }
        }
    }
    return this->current;
}
const void* vlist_get_const(vlist this, long index) {
    return vlist_get(this, index);
}
int vlist_add(vlist this, const void* node) {
    void* node_copy = malloc(this->node_size);
    if (node_copy == NULL)
    {
        return VLIST_ERROR_MALLOC_FAIL;
    }
    memcpy(node_copy, node, this->node_size);
    vlist_quick_add(this, node_copy);
    return 0;
}
void vlist_quick_add(vlist this, void* node) {
    if (this->size == 0)
    {
        ((__VLIST_NODE_STRUCT_TYPE*)node)->next = node;
        ((__VLIST_NODE_STRUCT_TYPE*)node)->prev = node;
        this->current = node;
        this->current_idx = 0;
    }
    else {
        __VLIST_NODE_STRUCT_TYPE* iptr = this->get(this, this->size - 1);
        ((__VLIST_NODE_STRUCT_TYPE*)(iptr->next))->prev = node;
        ((__VLIST_NODE_STRUCT_TYPE*)node)->next = iptr->next;
        ((__VLIST_NODE_STRUCT_TYPE*)node)->prev = iptr;
        iptr->next = node;
    }
    this->size++;
}
int vlist_insert(vlist this, long index, const void* node) {
    if (check_index(this, index) != 0)
    {
        return VLIST_ERROR_INVALID_INDEX;
    }
    void* node_copy = malloc(this->node_size);
    if (node_copy == NULL)
    {
        return VLIST_ERROR_MALLOC_FAIL;
    }
    memcpy(node_copy, node, this->node_size);
    return vlist_quick_insert(this, index, node_copy);
}
int vlist_quick_insert(vlist this, long index, void* node) {
    if (check_index(this, index) != 0)
    {
        return VLIST_ERROR_INVALID_INDEX;
    }
    __VLIST_NODE_STRUCT_TYPE* iptr = this->get(this, index);
    ((__VLIST_NODE_STRUCT_TYPE*)(iptr->prev))->next = node;
    ((__VLIST_NODE_STRUCT_TYPE*)node)->prev = iptr->prev;
    ((__VLIST_NODE_STRUCT_TYPE*)node)->next = iptr;
    iptr->prev = node;
    this->size++;
    this->current_idx++;
    return 0;
}
int vlist_remove(vlist this, long index) {
    if (check_index(this, index) != 0)
    {
        return VLIST_ERROR_INVALID_INDEX;
    }
    __VLIST_NODE_STRUCT_TYPE* iptr = this->get(this, index);
    this->current = iptr->prev;
    ((__VLIST_NODE_STRUCT_TYPE*)(iptr->prev))->next = iptr->next;
    ((__VLIST_NODE_STRUCT_TYPE*)(iptr->next))->prev = iptr->prev;
    free(iptr); iptr = NULL;
    this->size--;
    if (this->size == 0)
    {
        this->current = NULL;
        this->current_idx = -1;
    }
    else {
        this->current_idx--;
        if (this->current_idx < 0)
        {
            this->current_idx = this->size - 1;
        }
    }
    return 0;
}
void vlist_foreach(vlist this, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra) {
    for (long i = 0; i < this->size; i++)
    {
        if (run(this, i, extra) != 0) {
            break;
        }
    }
}
void vlist_foreach_reverse(vlist this, VLIST_RUNNABLE_FUNC_TYPE* run, void* extra) {
    for (long i = this->size - 1; i >= 0; i--)
    {
        if (run(this, i, extra) != 0) {
            break;
        }
    }
}
long vlist_flush(vlist this, VLIST_FILTER_FUNC_TYPE* filter, void* extra) {
    long flushed = 0;
    for (long i = this->size - 1; i >= 0; i--)
    {
        if (!filter(this, i, extra)) {
            this->remove(this, i);
            flushed++;
        }
    }
    return flushed;
}
void vlist_clear(vlist this) {
    while (this->size > 0)
    {
        this->remove(this, 0);
    }
}

vlist make_vlist(size_t node_size) {
    vlist res = malloc(sizeof(struct vlist));

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
