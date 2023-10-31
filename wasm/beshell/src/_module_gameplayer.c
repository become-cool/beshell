#include "_module_gameplayer.h"
#include "module_fs.h"
#include "utils.h"
#include "nofrendo.h"
#include <errno.h>


#include <time.h>
#include <math.h>
#include <string.h>

//Nes stuff wants to define this as well...
#undef false
#undef true
#undef bool

// #include "noftypes.h"
#include <bitmap.h>
#include <nofconfig.h>
#include <event.h>
#include <gui.h>
#include <log.h>
#include <nes.h>
#include <nes_pal.h>
#include <nesinput.h>
#include <osd.h>
#include <stdint.h>
#include <sys/stat.h>   

#define  VIDEO_WIDTH        256
#define  VIDEO_HEIGHT       240

static char * romdata = NULL ;

//Seemingly, this will be called only once. Should call func with a freq of frequency,
int osd_installtimer(int frequency, void *func, int funcsize, void *counter, int countersize) {
    printf("Timer install, freq=%d\n", frequency);

    // const esp_timer_create_args_t periodic_timer_args = {
    //     .callback = func,
    //     .name = "nes"
    // };
    // esp_timer_handle_t periodic_timer;
    // ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000*1000/frequency));


    // timer=xTimerCreate("nes",configTICK_RATE_HZ/frequency, pdTRUE, NULL, func);
    // xTimerStart(timer, 0);

    return 0;
}




void osd_setsound(void (*playfunc)(void *buffer, int length)) {
    printf("nes.osd_setsound()\n") ;
}

static void osd_stopsound(void) {
}

void osd_getsoundinfo(sndinfo_t *info) {
    printf("nes.osd_getsoundinfo()\n") ;
}

// /*
// ** Video
// */

static int init(int width, int height);
static void shutdown(void);
static int set_mode(int width, int height) {}
static void set_palette(rgb_t *pal);
static void clear(uint8 color);
static bitmap_t *lock_write(void);
static void free_write(int num_dirties, rect_t *dirty_rects);
static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects);
static char fb[1]; //dummy
static void * malloc_dma(int);


viddriver_t sdlDriver = {
   "MediaLayer",         /* name */
   init,          /* init */
   shutdown,      /* shutdown */
   set_mode,      /* set_mode */
   set_palette,   /* set_palette */
   clear,         /* clear */
   lock_write,    /* lock_write */
   free_write,    /* free_write */
   custom_blit,   /* custom_blit */
   malloc,
   false          /* invalidate flag */
};


bitmap_t * myBitmap ;

void osd_getvideoinfo(vidinfo_t *info) {
    printf("nes.osd_getvideoinfo()\n") ;
   info->default_width = VIDEO_WIDTH;
   info->default_height = VIDEO_HEIGHT;
   info->driver = &sdlDriver;
}

/* initialise video */
static int init(int width, int height) {
    printf("nes.init()\n") ;
	return 0;
}

static void shutdown(void) {
    printf("nes.shutdown()\n") ;
}


#define PALETTE_SIZE 256
uint16_t * palette = NULL ;
// rgb_t palette[256] ;

/* copy nes palette over to hardware */
static void set_palette(rgb_t *pal) {
}

/* clear all frames to a particular color */
static void clear(uint8 color) {
}



/* acquire the directbuffer for writing */
static bitmap_t *lock_write(void) {
    printf("lock_write\n") ;
//   SDL_LockSurface(mySurface);
   myBitmap = bmp_createhw((uint8*)fb, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH*2);
   return myBitmap;
    return NULL ;
}



/* release the resource */
static void free_write(int num_dirties, rect_t *dirty_rects) {
    printf("free_write\n") ;
//    bmp_destroy(&myBitmap);
}


static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects) {
    printf("custom_blit()\n") ;
}


char *osd_getromdata() {
    printf("osd_getromdata()") ;
    return romdata;
}


void osd_getinput(void) {
}


void osd_getmouse(int *x, int *y, int *button) {}

// /*
// ** Shutdown
// */

/* this is at the bottom, to eliminate warnings */
void osd_shutdown() {
}

static int logprint(const char *string) {
   return printf("nes:%s", string);
}

/*
** Startup
*/
int osd_init() {

    printf("osd_init()\n") ;
    if(!palette) {
        palette = malloc( sizeof(uint16)*PALETTE_SIZE ) ;
    }

	log_chain_logfunc(logprint);
	return 0;
}

int player_nofrendo_main() {
    return nofrendo_main(0, NULL);
}

static int player_nofrendo_read_rom(char * rompath) {
    
    struct stat statbuf;
    if(stat(rompath,&statbuf)<0) {
        printf("Failed to open rom file: %s\n", rompath);
        return false ;
    }
    if(!S_ISREG(statbuf.st_mode)) {
        printf("arg path is a file: %s\n", rompath);
        return false ;
    }
    if(statbuf.st_size<1){
        printf("rom file is empty: %s\n", rompath);
        return false ;
    }

    printf("rom size: %d\n", (int)statbuf.st_size) ;

	FILE * fd = fopen(rompath, "r");
    if(NULL==fd) {
        printf("Failed to open rome file (%d)\n", errno);
        return false ;
    }

    if(romdata) {
        free(romdata) ;
        romdata = NULL ;
    }
    
    romdata = malloc(statbuf.st_size) ;
    if(!romdata) {
        printf("out of memory?\n");
        fclose(fd) ;
        return false ;
    }

    size_t read_bytes = 1024 * 10 ;
    void * read_ptr = romdata ;
    size_t left_bytes = statbuf.st_size ;
    while( left_bytes>0) {

        if(read_bytes>left_bytes) {
            read_bytes = left_bytes ;
        }
        
        fread(read_ptr, 1, read_bytes, fd) ;

        read_ptr +=read_bytes ;
        left_bytes-=read_bytes ;
    }

    fclose(fd) ;
    return true ;
}


static JSValue js_gameplayer_test_rom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    char * rompath = js_arg_to_vfspath(ctx, argv[0]) ;
    if(!rompath) {
        THROW_EXCEPTION("invalid rom path")
    }

    if(!player_nofrendo_read_rom(rompath)) {
        free(rompath) ;
        THROW_EXCEPTION("can not open rom")
    }

    // ds(rompath)
    // free(rompath) ;

    nofrendo_main(0, NULL);

    return JS_UNDEFINED;
}


// void be_module_gameplayer_init() {
// }

void be_module_gameplayer_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue gameplayer = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "gameplayer", gameplayer);

    JS_SetPropertyStr(ctx, gameplayer, "testROM", JS_NewCFunction(ctx, js_gameplayer_test_rom, "testROM", 1));

    JS_FreeValue(ctx, beapi);
}

// void be_module_gameplayer_loop(JSContext *ctx) {
// }

// void be_module_gameplayer_reset(JSContext *ctx) {
// }

