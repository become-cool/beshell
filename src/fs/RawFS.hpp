#pragma once

#include "FSPartition.hpp"
#include <dirent.h>

#define MAX_OPEN_FD 16

namespace be {
        
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


    class RawFS: public FSPartition {
    public:
        void * ptr ;
        void * data_ptr ;
        size_t size ;
        vfs_node_t * root = nullptr ;
        vfs_rawfs_fd_t fds[MAX_OPEN_FD] ;
        RawFS(void * ptr, size_t size) ;
        bool mount(const char * mountPoint) ;
        void unmount() ;
        bool usage(size_t & total, size_t & used) ;
    } ;
}