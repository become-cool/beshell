#include "rawfs.h"

#ifdef PLATFORM_ESP32
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/lock.h>
#include <sys/param.h>
#include "esp_vfs.h"
#include "esp_partition.h"
#else
#include "module_fs.h"
#endif

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "debug.h"



// extern const uint8_t rawfs_root_start[] asm("_binary_root_raw_start");
// extern const uint8_t rawfs_root_end[] asm("_binary_root_raw_end");



static vfs_node_t * vfs_node_malloc(vfs_node_t * parent) {  
    vfs_node_t * node = malloc( sizeof(vfs_node_t) ) ;
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

vfs_node_t * parse_tree(char * raw, vfs_node_t * parent, char ** out_raw, size_t * out_total) {
    
    vfs_node_t * node = vfs_node_malloc(parent) ;
    if(!node) {
        return NULL ;
    }

    node->type = *raw ;
    raw ++ ;

    node->filename = raw ;
    raw+= strlen(node->filename) + 1 ;

#ifdef PLATFORM_LINUX
    // printf("%s\n",node->filename) ;
#endif

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

#define WARNING_READONLY(op) \
    printf("read only FS operation not supported: %s.\n",op) ;
#define RAWFS ((vfs_rawfs_t *)ctx)


#ifdef PLATFORM_ESP32

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

    memcpy(dst, RAWFS->raw_data + RAWFS->fds[fd].node->offset + RAWFS->fds[fd].offset, size) ;
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

    vfs_rawfs_dir_t * dir = malloc(sizeof(vfs_rawfs_dir_t)) ;
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

static vfs_rawfs_t * fs_root = NULL ;

#endif


void be_rawfs_mount(const char * mntPoint) {

	char * romdata = NULL ;

#ifdef PLATFORM_ESP32

	const esp_partition_t * part = esp_partition_find_first(0x01, 0x81, "fsroot");
    if(!part) {
        printf("not found partition: fsroot.\n") ;
        return ;
    }

    printf("root addr: 0x%x\n",(unsigned int)part->address) ;
    dn(part->size)
    ds(part->label)


    romdata = NULL ;
    
#if ESP_IDF_VERSION_MAJOR >= 5
	esp_partition_mmap_handle_t hrom ;
    esp_err_t ret = esp_partition_mmap(part, 0, part->size, ESP_PARTITION_MMAP_DATA, (const void**)&romdata, &hrom) ;
#else
	spi_flash_mmap_handle_t hrom ;
    esp_err_t ret = esp_partition_mmap(part, 0, part->size, SPI_FLASH_MMAP_DATA, (const void**)&romdata, &hrom) ;
#endif

	if(ret!= ESP_OK || !romdata) {
        printf("mmap failed: %d\n", ret) ;
        return ;
    }

    // print_block(romdata, 16, 8) ;
    
    fs_root = malloc(sizeof(vfs_rawfs_t)) ;
    fs_root->partition_size = part->size ;
    fs_root->size = 0 ;
    fs_root->raw = romdata ;
    fs_root->root = parse_tree(romdata, NULL, &fs_root->raw_data, &fs_root->size) ;
    fs_root->size+= (fs_root->raw_data-(void*)romdata) ; // raw header size

    memset(fs_root->fds, 0, sizeof(vfs_rawfs_fd_t)*MAX_OPEN_FD) ;

    const esp_vfs_t vfs = {
        .flags       = ESP_VFS_FLAG_CONTEXT_PTR,
        .write_p     = &vfs_rawfs_write,
        .pwrite_p    = &vfs_rawfs_pwrite,
        .lseek_p     = &vfs_rawfs_lseek,
        .read_p      = &vfs_rawfs_read,
        .pread_p     = &vfs_rawfs_pread,
        .open_p      = &vfs_rawfs_open,
        .close_p     = &vfs_rawfs_close,
        // .fstat_p     = &vfs_rawfs_fstat,
        .fstat_p     = NULL,
        .stat_p      = &vfs_rawfs_stat,
        .link_p      = NULL, /* Not Supported */
        .unlink_p    = &vfs_rawfs_unlink,
        .rename_p    = &vfs_rawfs_rename,
        .opendir_p   = &vfs_rawfs_opendir,
        .closedir_p  = &vfs_rawfs_closedir,
        .readdir_p   = &vfs_rawfs_readdir,
        .readdir_r_p = &vfs_rawfs_readdir_r,
        .seekdir_p   = &vfs_rawfs_seekdir,
        .telldir_p   = &vfs_rawfs_telldir,
        .mkdir_p     = &vfs_rawfs_mkdir,
        .rmdir_p     = &vfs_rawfs_rmdir,
        .fsync_p     = &vfs_rawfs_fsync,
        // .utime_p     = &vfs_rawfs_utime,
        .utime_p     = NULL,
    };

    esp_err_t err = esp_vfs_register(mntPoint, &vfs, fs_root);
    if (err != ESP_OK) {
        printf("Failed to register tar fs");
        return ;
    }

#elif defined(PLATFORM_LINUX)

    char * tarpath = vfspath_to_fs("../img/fs-root.img") ;

    struct stat statbuf;
    if(stat(tarpath,&statbuf)<0) {
        printf("Failed to stat file: %s\n", tarpath);
        free(tarpath) ;
        return ;
    }

    // dn(statbuf.st_size)

    romdata = malloc(statbuf.st_size) ;
    if(!romdata) {
        printf("malloc failed: %d\n",statbuf.st_size) ;
        return ;
    }

    FILE * file = fopen(tarpath, "r") ;
    if(!file) {
        printf("open file failed: %s", tarpath);
        return ;
    }

    fread(romdata, 1, statbuf.st_size, file) ;

    fclose(file) ;
    free(tarpath) ;

    vfs_node_t * root = parse_tree(romdata, NULL, NULL, NULL) ;

    vfs_node_printf(root, "") ;

    // print_block(romdata, 8, 8) ;
#endif

}


#ifdef PLATFORM_ESP32
vfs_rawfs_t * be_rawfs_root() {
    return fs_root ;
}
#endif

// esp_partition_read
