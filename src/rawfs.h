#ifndef ONCE_H_RAWFS
#define ONCE_H_RAWFS

#ifdef PLATFORM_ESP32
#include "esp_system.h"
#else
#include <stdint.h>
#endif

#include <dirent.h>

#define MAX_OPEN_FD 16

#define VFS_NODE_DIR 1
#define VFS_NODE_FILE 2

typedef struct _vfs_node {

    unsigned char type ;
    char * filename ;
    uint32_t offset ;
    uint32_t filesize ;

    struct _vfs_node * next ;
    struct _vfs_node * children ;

} vfs_node_t ;


#ifdef PLATFORM_ESP32

typedef struct {
    DIR dir;            /*!< VFS DIR struct */
    struct dirent e;    /*!< Last open dirent */

    vfs_node_t * node ;
    vfs_node_t * walk_child ;

} vfs_rawfs_dir_t;


typedef struct {
    vfs_node_t * node ;
    uint32_t offset ;
} vfs_rawfs_fd_t;


typedef struct _vfs_rawfs {
    vfs_node_t * root ;
    void * raw ;
    void * raw_data ;
    size_t size ;
    size_t partition_size ;

    vfs_rawfs_fd_t fds[MAX_OPEN_FD] ;

} vfs_rawfs_t ;

vfs_rawfs_t * be_rawfs_root() ;

#endif


void be_rawfs_mount(const char * mntPoint) ;

#endif