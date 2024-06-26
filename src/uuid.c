#include "uuid.h"

#include <string.h>
#include <sys/time.h>

#ifdef ESP_PLATFORM
#include "esp_system.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void random_(uint8_t * data, size_t len) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand((unsigned) tv.tv_usec);
    while(len--) {
        (*data) = rand() % 256 ;
        data++ ;
    }
}
#endif



void uuid_generate(uuid_t out) {
#ifdef ESP_PLATFORM
    esp_fill_random((uint8_t *)out, sizeof(uuid_t));
#else
    random_((uint8_t *)out, sizeof(uuid_t));
#endif
    out[6] = 0x40 | (out[6] & 0xF);
    out[8] = (0x80 | out[8]) & ~0x40;
}


static uint8_t unhex_char(unsigned char s)
{
    if (0x30 <= s && s <= 0x39) { /* 0-9 */
        return s - 0x30;
    } else if (0x41 <= s && s <= 0x46) { /* A-F */
        return s - 0x41 + 0xa;
    } else if (0x61 <= s && s <= 0x69) { /* a-f */
        return s - 0x61 + 0xa;
    } else {
        /* invalid string */
        return 0xff;
    }
}

static int unhex(unsigned char *s, size_t s_len, unsigned char *r)
{
    int i;

    for (i = 0; i < s_len; i += 2) {
        uint8_t h = unhex_char(s[i]);
        uint8_t l = unhex_char(s[i + 1]);
        if (0xff == h || 0xff == l) {
            return -1;
        }
        r[i / 2] = (h << 4) | (l & 0xf);
    }

    return 0;
}

int uuid_parse(const char *in, uuid_t uu)
{
    const char *p = in;
    uint8_t *op = (uint8_t *)uu;

    if (0 != unhex((unsigned char *)p, 8, op)) {
        return -1;
    }
    p += 8;
    op += 4;

    for (int i = 0; i < 3; i++) {
        if ('-' != *p++ || 0 != unhex((unsigned char *)p, 4, op)) {
            return -1;
        }
        p += 4;
        op += 2;
    }

    if ('-' != *p++ || 0 != unhex((unsigned char *)p, 12, op)) {
        return -1;
    }
    p += 12;
    op += 6;

    return 0;
}

void uuid_unparse(const uuid_t uu, char *out)
{
    snprintf(out, UUID_STR_LEN,
             "%02x%02x%02x%02x-%02x%02x-%02x%02x-"
             "%02x%02x-%02x%02x%02x%02x%02x%02x",
             uu[0], uu[1], uu[2], uu[3], uu[4], uu[5], uu[6], uu[7], uu[8], uu[9], uu[10], uu[11],
             uu[12], uu[13], uu[14], uu[15]);
}
