#include "sf33rd/Source/PS2/mc/msgsub.h"
#include "common.h"
#include "sf33rd/Source/PS2/mc/knjsub.h"
#include "sf33rd/Source/PS2/mc/msgtble.h"
#include "sf33rd/Source/PS2/mc/msgtblj.h"

#include <stdio.h>

// sdata
static void* msg_table_ptr[2] = { MessageTableJPN, MessageTableENG };

static u32 str_col_tbl[8] = { 0x00000000, 0x00000080, 0x00008000, 0x00008080,
                              0x00800000, 0x00800080, 0x00808000, 0x00808080 };

// sbss
s32 MsgLanguage;

// forward decls
static const s8* str_tag_get(const s8* src, s8* dst);
static const s8* str_str_get(const s8* src, s8* dst);
static void str_tag_check(const s8* tag);

s8** GetMemCardMsg(s32 msg_no) {
    void** tbl = msg_table_ptr[MsgLanguage];
    void** msg = *tbl;

    return msg[msg_no];
}

s32 FormStrDisp(s32 x, s32 y, const s8* str, s32 len) {
    u8 c;
    s32 ctr = 0;
    s32 i;
    s32 ret;
    const s8* p;
    s8 tmp[128];
    s8* q;
    s8 tmp2[4];

    (void)i;

    KnjLocate(x, y);

    while (*str != '\0') {
        if ((p = str_tag_get(str, tmp)) == NULL) {
            str = str_str_get(str, tmp);

            if (len == 0) {
                KnjPuts(tmp);
                continue;
            }

            p = tmp;
            q = tmp2;

            while ((c = *p)) {
                ret = KnjCheckCode(p) & 3;

                for (i = 0; i < ret; i++) {
                    *q++ = *p++;
                }

                *q = '\0';
                KnjPuts(tmp2);
                q = tmp2;
                ctr += 1;

                if (ctr >= len) {
                    return ctr;
                }
            }
        } else {
            str_tag_check(tmp);
            str = p;
        }
    }

    return 0;
}

static const s8* str_tag_get(const s8* src, s8* dst) {
    u8 c;

    if (*src != '<') {
        return NULL;
    }

    src++;

    while (1) {
        c = *src++;

        if (c == '\0') {
            return NULL;
        }

        if (c == '>') {
            break;
        }

        *dst++ = c;
    }

    *dst = '\0';
    return src;
}

static const s8* str_str_get(const s8* src, s8* dst) {
    u8 c;
    s32 i;
    s32 ret;

    while ((c = *src)) {
        ret = KnjCheckCode(src) & 3;

        if ((ret == 1) && c == '<') {
            break;
        }

        for (i = 0; i < ret; i++) {
            *dst++ = *src++;
        }
    }

    *dst = '\0';
    return src;
}

static void str_tag_check(const s8* tag) {
    u8 c;
    const s8* tag0 = tag;

    switch ((c = *tag++)) {
    default:
        goto err;

    case 'C':
        break;
    }

    c = *tag++;

    if (c == '\0') {
        goto err;
    }

    c -= 0x30;

    if (c >= 8) {
        goto err;
    }

    KnjSetRgb(str_col_tbl[c]);
    return;

err:
    printf("message format error, <%s>\n.", tag0);

    while (1) {
        // Do nothing
    }
}
