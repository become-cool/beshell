#include "RawFS.hpp"
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/lock.h>
#include <sys/param.h>
#include <stdlib.h>
#include "esp_vfs.h"
#include "esp_partition.h"
#include <iostream>
#include "debug.h"

using namespace std ;

namespace be {

    static vfs_node_t * vfs_node_malloc(vfs_node_t * parent) {  
        vfs_node_t * node = (vfs_node_t *)malloc( sizeof(vfs_node_t) ) ;
        memset(node, 0, sizeof(vfs_node_t) );

        if(parent) {
            node->next = parent->children ;
            parent->children = node ;
        }

        return node ;
    }

    static void free_node(vfs_node_t * node) {
        free(node) ;
    }

    static vfs_node_t * vfs_node_get_child_by_name(vfs_node_t * parent, const char * name, size_t namelen) {
        for(vfs_node_t * child=parent->children; child!=NULL; child=child->next) {
            if(strlen(child->filename)==namelen && strncmp(child->filename, name, namelen) == 0) {
                return child;
            }
        }
        return NULL ;
    }


    static vfs_node_t * vfs_node_walk_path(vfs_node_t * parent, const char * path, size_t len) {
    // ds(path)
    // dn(len)

        // 跳过开头的 /
        if(path[0]=='/') {
            path ++ ;
            len -- ;
        }
        if(len==0) {
            // ds(parent->filename)
            return parent ;
        }
        int namelen = 0 ;
        char * p = strchr(path,'/') ;
        if(p) {
            namelen = p - path ;
        }
        else {
            namelen = len ;
        }

        if( namelen==0 || (namelen==1 && path[0]=='.') ) {
            return vfs_node_walk_path(parent, p, len-namelen) ;
        }
        
        // char e = path[namelen] ;
        // path[namelen] = 0 ;

        vfs_node_t * node = vfs_node_get_child_by_name(parent, path, namelen) ;
        if(node==NULL) {
            // printf("return null");
            return NULL ;
        }

        // path[namelen] = e ;

        // 叶节点
        if(!p) {
            // printf("%s, offset=%d, size=%d\n", node->filename, node->offset, node->filesize) ;
            // ds(node->filename)
            // dn(node->offset)
            // dn(node->filesize)
            return node ;
        }
        
        len-= namelen ;

        if(*p=='/'){
            p ++ ;
            len -- ;
        }
        if(len<=0) {
            // over
            // ds(node->filename)
            return node ;
        }

        // 递归
        return vfs_node_walk_path(node, p, len) ;
    }

    static void vfs_node_printf(vfs_node_t * node, const char * path) {

    }


    #define WARNING_READONLY(op) \
        printf("read only FS operation not supported: %s.\n",op) ;
    #define RAWFS ((RawFS *)ctx)
    
    static int vfs_rawfs_open(void* ctx, const char * path, int flags, int mode) {
        vfs_node_t * node = vfs_node_walk_path(RAWFS->root, path, strlen(path));
        if(!node) {
            return -1 ;
        }
        if( node->type != VFS_NODE_FILE ){
            return -2 ;
        }

        uint8_t fd = 0 ;
        for(;fd<MAX_OPEN_FD;fd++) {
            if( ! RAWFS->fds[fd].node) {
                break ;
            }
        }

        if(fd>=MAX_OPEN_FD) {
            printf("too much file opened: %d\n", MAX_OPEN_FD) ;
            return -3 ;
        }

        RAWFS->fds[fd].node = node ;
        RAWFS->fds[fd].offset = 0 ;

        return  fd ;
    }
    static ssize_t vfs_rawfs_write(void* ctx, int fd, const void * data, size_t size) {
        WARNING_READONLY("write")
        return -1 ;
    }
    static ssize_t vfs_rawfs_read(void* ctx, int fd, void * dst, size_t size) {
        if(fd<0 || fd>=MAX_OPEN_FD) {
            return -1 ;
        }
        if(!RAWFS->fds[fd].node) {
            return -2 ;
        }
        if(RAWFS->fds[fd].offset + size > RAWFS->fds[fd].node->filesize) {
            size = RAWFS->fds[fd].node->filesize - RAWFS->fds[fd].offset ;
        }

        // printf("read(fd:%d): offset=%d, size=%d\n", fd, RAWFS->fds[fd].node->offset + RAWFS->fds[fd].offset, size) ;

        memcpy(dst, RAWFS->data_ptr + RAWFS->fds[fd].node->offset + RAWFS->fds[fd].offset, size) ;
        RAWFS->fds[fd].offset+= size ;

        return size ;
    }
    static ssize_t vfs_rawfs_pwrite(void *ctx, int fd, const void *src, size_t size, off_t offset) {
        WARNING_READONLY("pwrite")
        return -1 ;
    }
    static int vfs_rawfs_close(void* ctx, int fd) {
        if(fd<0 || fd>=MAX_OPEN_FD) {
            return -1 ;
        }
        // dfunc
        RAWFS->fds[fd].node = NULL ;
        RAWFS->fds[fd].offset = 0 ;
        return 0 ;
    }
    static off_t vfs_rawfs_lseek(void* ctx, int fd, off_t offset, int mode) {
        if(fd<0 || fd>=MAX_OPEN_FD) {
            return -1 ;
        }

        if(!RAWFS->fds[fd].node) {
            return -2 ;
        }
        if( offset > RAWFS->fds[fd].node->filesize ) {
            return -3 ;
        }

        if(mode==SEEK_SET) {
            RAWFS->fds[fd].offset = offset ;
        }
        else if(mode==SEEK_CUR) {
            RAWFS->fds[fd].offset+= offset ;
        }
        else if(mode==SEEK_END) {
            if(offset<=0) {
                RAWFS->fds[fd].offset = RAWFS->fds[fd].node->filesize + offset ;
            }
        }
        
        return RAWFS->fds[fd].offset ;
    }
    static ssize_t vfs_rawfs_pread(void *ctx, int fd, void *dst, size_t size, off_t offset) {
        vfs_rawfs_lseek(ctx, fd, offset, 0) ;
        return vfs_rawfs_read(ctx,fd,dst,size) ;
    }
    static int vfs_rawfs_stat(void* ctx, const char * path, struct stat * st) {
        vfs_node_t * node = vfs_node_walk_path(RAWFS->root, path, strlen(path)) ;
        if(!node) {
            return -1 ;
        }

        st->st_size = node->filesize ;
        st->st_mode = ((node->type==VFS_NODE_FILE)?S_IFREG:S_IFDIR);
        st->st_atime = 0;
        st->st_mtime = 0;
        st->st_ctime = 0;
        st->st_dev = 0;
        st->st_ino = 0;
        
        return 0 ;
    }
    static int vfs_rawfs_unlink(void* ctx, const char *path) {
        WARNING_READONLY("unlink")
        return -1 ;
    }
    static int vfs_rawfs_rename(void* ctx, const char *src, const char *dst) {
        WARNING_READONLY("rename")
        return -1 ;
    }
    static DIR* vfs_rawfs_opendir(void* ctx, const char* name) {
        vfs_node_t * dirnode = vfs_node_walk_path(RAWFS->root, name, strlen(name)) ;
        if(!dirnode) {
            return NULL ;
        }

        vfs_rawfs_dir_t * dir = (vfs_rawfs_dir_t *)malloc(sizeof(vfs_rawfs_dir_t)) ;
        memset(dir, 0, sizeof(vfs_rawfs_dir_t)) ;
        dir->node = dirnode ;
        dir->walk_child = dirnode->children ;

        return (DIR*)dir ;
    }
    static int vfs_rawfs_closedir(void* ctx, DIR* pdir) {
        if(!pdir) {
            return  0 ;
        }
        free(pdir) ;
        return 0 ;
    }
    static int vfs_rawfs_readdir_r(void* ctx, DIR* pdir, struct dirent* entry, struct dirent** out_dirent) { 
        vfs_rawfs_dir_t * dir = (vfs_rawfs_dir_t*) pdir ;
        if(!dir->walk_child) {
            *out_dirent = NULL ;
            return 0 ;
        }
        
        strcpy(entry->d_name, dir->walk_child->filename) ;
        entry->d_type = dir->walk_child->type ;

        (*out_dirent) = entry ;
        
        dir->walk_child = dir->walk_child->next ;

        return 0 ;
    }
    static struct dirent* vfs_rawfs_readdir(void* ctx, DIR* pdir) {
        if(!pdir) {
            return  0 ;
        }
        struct dirent * out_dirent ;
        if(vfs_rawfs_readdir_r(RAWFS->root, pdir, &(((vfs_rawfs_dir_t*) pdir)->e), &out_dirent)!=0) {
            return NULL ;
        }

        return out_dirent ;
    }
    static long vfs_rawfs_telldir(void* ctx, DIR* pdir) {
        WARNING_READONLY("telldir")
        return 0 ;
    }
    static void vfs_rawfs_seekdir(void* ctx, DIR* pdir, long offset) {
        WARNING_READONLY("seekdir")
        return ;
    }
    static int vfs_rawfs_mkdir(void* ctx, const char* name, mode_t mode) {
        WARNING_READONLY("mkdir")
        return 0 ;
    }
    static int vfs_rawfs_rmdir(void* ctx, const char* name) {
        WARNING_READONLY("rmdir")
        return 0 ;
    }
    static int vfs_rawfs_fsync(void* ctx, int fd) {
        WARNING_READONLY("fsync")
        return 0 ;
    }


    vfs_node_t * parse_tree(char * raw, vfs_node_t * parent, char ** out_raw, size_t * out_total) {
        
        vfs_node_t * node = vfs_node_malloc(parent) ;
        if(!node) {
            return NULL ;
        }

        node->type = *raw ;
        raw ++ ;

        node->filename = raw ;
        raw+= strlen(node->filename) + 1 ;

        node->filesize = * ((uint32_t *)raw) ;
        raw+= sizeof(uint32_t) ;

        if(node->type==VFS_NODE_DIR) {
            for(int i=0;i<node->filesize;i++) {
                parse_tree(raw, node, &raw, out_total) ;
            }
        }
        else if(node->type==VFS_NODE_FILE) {
            node->offset = * ((uint32_t *)raw) ;
            raw+= sizeof(uint32_t) ;
        }
        else {
            printf("unknown node type: %d\n", node->type) ;
            return NULL ;
        }
        // printf("%s\n, size:%d, offset:%d\n",node->filename,node->filesize,node->offset) ;

        if(out_total) {
            (*out_total)+= node->filesize ;
        }
        if(out_raw) {
            *out_raw = raw ;
        }

        return node ;
    }

    RawFS::RawFS(void * _ptr, size_t _size)
        : ptr(_ptr)
        , size(_size)
    {}
    bool RawFS::mount(const char * _mountPoint) {

        root = parse_tree((char *)ptr, NULL, (char**)&data_ptr, NULL) ;

        memset(fds, 0, sizeof(vfs_rawfs_fd_t)*MAX_OPEN_FD) ;

        esp_vfs_t vfs ;
        memset(&vfs, 0, sizeof(esp_vfs_t)) ;
        vfs.flags       = ESP_VFS_FLAG_CONTEXT_PTR;
        vfs.write_p     = &vfs_rawfs_write;
        vfs.pwrite_p    = &vfs_rawfs_pwrite;
        vfs.lseek_p     = &vfs_rawfs_lseek;
        vfs.read_p      = &vfs_rawfs_read;
        vfs.pread_p     = &vfs_rawfs_pread;
        vfs.open_p      = &vfs_rawfs_open;
        vfs.close_p     = &vfs_rawfs_close;
        // vfs.fstat_p     = &vfs_rawfs_fstat;
        vfs.fstat_p     = NULL;
        vfs.stat_p      = &vfs_rawfs_stat;
        vfs.link_p      = NULL; /* Not Supported */
        vfs.unlink_p    = &vfs_rawfs_unlink;
        vfs.rename_p    = &vfs_rawfs_rename;
        vfs.opendir_p   = &vfs_rawfs_opendir;
        vfs.closedir_p  = &vfs_rawfs_closedir;
        vfs.readdir_p   = &vfs_rawfs_readdir;
        vfs.readdir_r_p = &vfs_rawfs_readdir_r;
        vfs.seekdir_p   = &vfs_rawfs_seekdir;
        vfs.telldir_p   = &vfs_rawfs_telldir;
        vfs.mkdir_p     = &vfs_rawfs_mkdir;
        vfs.rmdir_p     = &vfs_rawfs_rmdir;
        vfs.fsync_p     = &vfs_rawfs_fsync;
        // vfs.utime_p     = &vfs_rawfs_utime;
        vfs.utime_p     = NULL;

        esp_err_t err = esp_vfs_register(_mountPoint, &vfs, this);
        if (err != ESP_OK) {
            std::cout << "Failed to register tar fs" << std::endl ;
            return false ;
        }

        mountPoint = _mountPoint ;
        return true ;
    }
    void RawFS::unmount() {
        cout << "not implements" <<endl ;
    }
}