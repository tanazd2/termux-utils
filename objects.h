#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OBJ_LINES 1024
#define MAX_LINE_LEN  256
#define MAX_OBJ_KV    128

typedef struct ObjKV {
    char key[64];
    char value[256];
} ObjKV;

typedef struct Obj {
    char name[50];
    char lines[MAX_OBJ_LINES][MAX_LINE_LEN];
    int line_count;

    ObjKV kvs[MAX_OBJ_KV];
    int kv_count;

    void (*run)(struct Obj*, int argc, char** argv);
} Obj;

// Add line
#define line(text) do { \
    if (_obj_ptr->line_count < MAX_OBJ_LINES) \
        strncpy(_obj_ptr->lines[_obj_ptr->line_count++], text, MAX_LINE_LEN-1); \
} while(0)

// Add key-value
#define kvs(key_str, value_str) do { \
    if (_obj_ptr->kv_count < MAX_OBJ_KV) { \
        strncpy(_obj_ptr->kvs[_obj_ptr->kv_count].key, key_str, 63); \
        strncpy(_obj_ptr->kvs[_obj_ptr->kv_count].value, value_str, 255); \
        _obj_ptr->kv_count++; \
    } \
} while(0)

// Attach command
#define cmd(func) (_obj_ptr->run = func)

// Object definition
#define obj(obj_name) \
    Obj obj_##obj_name = { .name = #obj_name, .line_count = 0, .kv_count = 0, .run = NULL }; \
    for (Obj* _obj_ptr = &obj_##obj_name; _obj_ptr; _obj_ptr = NULL)

// Append more lines/data
#define objadd(obj_name) \
    for (Obj* _obj_ptr = &obj_##obj_name; _obj_ptr; _obj_ptr = NULL)

// Run macros
#define objrunve(obj_name, argv) do { if (obj_##obj_name.run) obj_##obj_name.run(&obj_##obj_name, 0, argv); } while(0)

#define objrune(obj_name, ...) do { \
    char* args[] = {__VA_ARGS__, NULL}; \
    if (obj_##obj_name.run) obj_##obj_name.run(&obj_##obj_name, sizeof(args)/sizeof(args[0])-1, args); \
} while(0)

#define objrunvp(obj_name, argv) objrunve(obj_name, argv)
#define objrunvpe(obj_name, argv) objrunve(obj_name, argv)
#define objrunpe(obj_name, argv) objrunve(obj_name, argv)

#endif // OBJECTS_H
