#include "sf33rd/AcrSDK/ps2/flps2dma.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include <eekernel.h>
#include <eeregs.h>
#include <libdma.h>
#include <libgraph.h>

// sbss
u64 flPs2StoreImageOldIMR;
static u32 flPs2StoreImageSize;
static uintptr_t flPs2StoreImageAdrs;

static __int128 vif1_fifo = (__int128)0x06000000;
static u32* dma_chcr_adrs[10] = { (u32*)0x10008000, (u32*)0x10009000, (u32*)0x1000A000, (u32*)0x1000B000,
                                  (u32*)0x1000B400, (u32*)0x1000C000, (u32*)0x1000C400, (u32*)0x1000C800,
                                  (u32*)0x1000D000, (u32*)0x1000D400 };

uintptr_t flPS2DmaAddCntTag(uintptr_t tag, s32 qwc, s32 irq, s32 /*unused*/) {
    QWORD* wk_ptr = (QWORD*)tag;
    wk_ptr->UI128 = 0;
    wk_ptr->UI32[0] = qwc + 0x10000000;

    if (irq == 1) {
        wk_ptr->UI32[0] |= 0x80000000;
    }

    return tag + 0x10;
}

uintptr_t flPS2DmaAddRefTag(uintptr_t tag, s32 qwc, uintptr_t data_adrs, s32 irq, s32 /*unused*/) {
    QWORD* wk_ptr = (QWORD*)tag;
    uintptr_t addr = data_adrs & 0xFFFFFFF;
    u32 spr_flag = 0;

    if ((data_adrs & 0x70000000) == 0x70000000) {
        spr_flag = 0x80000000;
    }

    wk_ptr->UI128 = 0;
    wk_ptr->UI32[0] = qwc + 0x30000000;
    wk_ptr->UI32[1] = (u32)(addr | spr_flag);

    if (irq == 1) {
        wk_ptr->UI32[0] |= 0x80000000;
    }

    return tag + 0x10;
}

uintptr_t flPS2DmaAddRefeTag(uintptr_t tag, s32 qwc, uintptr_t data_adrs, s32 irq, s32 /*unused*/) {
    QWORD* wk_ptr = (QWORD*)tag;
    uintptr_t addr = data_adrs & 0xFFFFFFF;
    u32 spr_flag = 0;

    if ((data_adrs & 0x70000000) == 0x70000000) {
        spr_flag = 0x80000000;
    }

    wk_ptr->UI128 = 0;
    wk_ptr->UI32[0] = qwc;
    wk_ptr->UI32[1] = (u32)(addr | spr_flag);

    if (irq == 1) {
        wk_ptr->UI32[0] |= 0x80000000;
    }

    return tag + 0x10;
}

uintptr_t flPS2DmaAddEndTag(uintptr_t tag, s32 qwc, s32 irq, s32 /*unused*/) {
    QWORD* wk_ptr = (QWORD*)tag;

    wk_ptr->UI128 = 0;
    wk_ptr->UI32[0] = qwc + 0x70000000;

    if (irq == 1) {
        wk_ptr->UI32[0] |= 0x80000000;
    }

    return tag + 0x10;
}

uintptr_t flPS2VIF1CodeAddDirectHL(uintptr_t tag, s32 qwc) {
    QWORD* pkt = (QWORD*)tag;

    pkt->UI32[0] = 0;
    pkt->UI32[1] = 0;
    pkt->UI32[2] = 0;
    pkt->UI32[3] = qwc | 0x51000000;

    return tag + 0x10;
}

u32 flPS2VIF1CalcLoadImageSize(u32 size) {
    u32 count;
    u32 data_size;

    data_size = 0;
    count = size / 0x70000;

    if ((size % 0x70000) != 0) {
        count += 1;
    }

    data_size = count << 7;

    return data_size;
}

u32 flPS2VIF1CalcEndLoadImageSize(s32 /*unused*/) {
    return 0x40;
}

uintptr_t flPS2VIF1MakeLoadImage(uintptr_t buff_ptr, u32 irq, uintptr_t data_ptr, u32 size, s16 dbp, s16 dbw, s16 dpsm,
                                 s16 x, s16 y, s16 w, s16 h) {
    u_long* work_ptr;
    u32 lp0;
    u32 count;
    u32 trans_size;
    s16 wk_h;
    FLPS2LoadImageData* load_image;
    uintptr_t last_tag;

    last_tag = 0;
    count = size / 0x70000;

    if ((size % 0x70000) != 0) {
        count += 1;
    }

    if (count == 1) {
        wk_h = h;
    } else {
        switch (dpsm) {
        case SCE_GS_PSMCT32:
        case SCE_GS_PSMCT24:
            wk_h = 0x70000 / (w * 4);
            break;

        case SCE_GS_PSMCT16:
        case SCE_GS_PSMCT16S:
            wk_h = 0x70000 / (w * 2);
            break;

        case SCE_GS_PSMT8:
            wk_h = 0x70000 / w;
            break;

        case SCE_GS_PSMT4:
            wk_h = 0x70000 / (w >> 1);
            break;
        }
    }

    if (dbw == 0) {
        dbw = 1;
    }

    work_ptr = (u_long*)buff_ptr;

    for (lp0 = 0; lp0 < count; lp0++) {
        switch (dpsm) {
        case SCE_GS_PSMCT32:
        case SCE_GS_PSMCT24:
            trans_size = w * wk_h * 4;
            break;

        case SCE_GS_PSMCT16:
        case SCE_GS_PSMCT16S:
            trans_size = w * wk_h * 2;
            break;

        case SCE_GS_PSMT8:
            trans_size = w * wk_h;
            break;

        case SCE_GS_PSMT4:
            trans_size = (w * wk_h) / 2;
            break;
        }

        load_image = (FLPS2LoadImageData*)work_ptr;
        flPS2DmaAddCntTag((uintptr_t)load_image, 6, 0, 0);
        load_image->dmatag.data.UI32[2] = 0x13000000;
        load_image->dmatag.data.UI32[3] = 0x51000006;

        load_image->giftag.data.UI64[0] = SCE_GIF_SET_TAG(4, 0, 0, 0, SCE_GIF_PACKED, 1);
        load_image->giftag.data.UI64[1] = SCE_GIF_PACKED_AD;

        load_image->bitbltbuf.UI64[0] = SCE_GS_SET_BITBLTBUF(0, 0, 0, dbp, dbw, dpsm);
        load_image->bitbltbuf.UI64[1] = SCE_GS_BITBLTBUF;

        load_image->trxpos.UI64[0] = SCE_GS_SET_TRXPOS(0, 0, x, y, 0);
        load_image->trxpos.UI64[1] = SCE_GS_TRXPOS;

        load_image->trxreg.UI64[0] = SCE_GS_SET_TRXREG(w, wk_h);
        load_image->trxreg.UI64[1] = SCE_GS_TRXREG;

        load_image->trxdir.UI64[0] = SCE_GS_SET_TRXDIR(0);
        load_image->trxdir.UI64[1] = SCE_GS_TRXDIR;

        if (lp0 >= count - 1) {
            load_image->giftag1.data.UI64[0] = SCE_GIF_SET_TAG(size / 16, 1, 0, 0, SCE_GIF_IMAGE, 0);
            load_image->giftag1.data.UI64[1] = 0;
            flPS2DmaAddRefeTag((uintptr_t)load_image + 0x70, size / 16, data_ptr, irq, 0);

            load_image->dmatag1.data.UI32[2] = 0;
            load_image->dmatag1.data.UI32[3] = (size / 16) | 0x51000000;
            last_tag = (uintptr_t)load_image + 0x70;
        } else {
            load_image->giftag1.data.UI64[0] = SCE_GIF_SET_TAG(0xF000, 0, 0, 0, 2, 0);
            load_image->giftag1.data.UI64[1] = 0;
            flPS2DmaAddRefTag((uintptr_t)load_image + 0x70, 0x7000, data_ptr, 0, 0);

            load_image->dmatag1.data.UI32[2] = 0;
            load_image->dmatag1.data.UI32[3] = 0x51000000 | 0x7000;
            work_ptr = (u_long*)load_image + 0x10;
            data_ptr += trans_size;
            y += wk_h;

            if (y + wk_h > h) {
                wk_h = h - y;
            }

            size -= trans_size;
        }
    }

    return last_tag;
}

void flPS2VIF1MakeEndLoadImage(uintptr_t buff_ptr, u32 irq) {
    FLPS2LoadEndData* load_end = (FLPS2LoadEndData*)buff_ptr;
    flPS2DmaAddEndTag((uintptr_t)load_end, 3, irq, 0);
    flPS2VIF1CodeAddDirectHL((uintptr_t)load_end + 0x10, 2);

    load_end->giftag.UI64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, SCE_GIF_PACKED, 1);
    load_end->giftag.UI64[1] = SCE_GIF_PACKED_AD;

    load_end->texflush.UI64[0] = 0;
    load_end->texflush.UI64[1] = SCE_GS_TEXFLUSH;
}

void flPS2StoreImageB(uintptr_t load_ptr, u32 size, s16 dbp, s16 dbw, s16 dpsm, s16 x, s16 y, s16 w, s16 h) {
    sceDmaChan* dma_channel;
    u32 lp0;
    u32 count;
    u32 trans_size;
    s16 wk_h;
    FLPS2StoreImageData* store_image;

    flPS2DmaWait();
    count = size / 0x70000;

    if ((size % 458752) != 0) {
        count += 1;
    }

    if (count == 1) {
        wk_h = h;
    } else {
        switch (dpsm) {
        case 0:
        case 1:
            wk_h = 0x70000 / (w * 4);
            break;

        case 2:
        case 10:
            wk_h = 0x70000 / (w * 2);
            break;

        case 19:
            wk_h = 0x70000 / w;
            break;

        case 20:
            wk_h = 0x70000 / (w >> 1);
            break;
        }
    }

    if (dbw == 0) {
        dbw = 1;
    }

    dma_channel = flPs2State.DmaChan[flPs2VIF1Control.channel_id];

    for (lp0 = 0; lp0 < count; lp0++) {
        switch (dpsm) {
        case 0:
        case 1:
            trans_size = w * wk_h * 4;
            break;

        case 2:
        case 10:
            trans_size = w * wk_h * 2;
            break;

        case 19:
            trans_size = w * wk_h;
            break;

        case 20:
            trans_size = (w * wk_h) / 2;
            break;
        }

        store_image = (FLPS2StoreImageData*)flPS2GetSystemTmpBuff(0x90, 0x10);
        flPS2DmaAddEndTag((uintptr_t)store_image, 7, 0, 0);
        store_image->dmatag.data.UI32[2] = 0;
        store_image->dmatag.data.UI32[3] = 0;

        store_image->vifcode.UI32[0] = SCE_VIF1_SET_NOP(0);
        store_image->vifcode.UI32[1] = 0x06008000; // Not sure what macro to use here , if possible
        store_image->vifcode.UI32[2] = 0x13000000; // Not sure what macro to use here, if possible

        store_image->vifcode.UI32[3] = SCE_VIF1_SET_DIRECT(6, 0);

        store_image->giftag.UI64[0] = SCE_GIF_SET_TAG(5, 1, 0, 0, SCE_GIF_PACKED, 1);
        store_image->giftag.UI64[1] = SCE_GIF_PACKED_AD;

        store_image->bitbltbuf.UI64[0] = SCE_GS_SET_BITBLTBUF(dbp, dbw, dpsm, 0, 0, 0);
        store_image->bitbltbuf.UI64[1] = SCE_GS_BITBLTBUF;

        store_image->trxpos.UI64[0] = SCE_GS_SET_TRXPOS(x, y, 0, 0, 0);
        store_image->trxpos.UI64[1] = SCE_GS_TRXPOS;

        store_image->trxreg.UI64[0] = SCE_GS_SET_TRXREG(w, wk_h);
        store_image->trxreg.UI64[1] = SCE_GS_TRXREG;

        store_image->finish.UI64[0] = 0;
        store_image->finish.UI64[1] = SCE_GS_FINISH;

        store_image->trxdir.UI64[0] = SCE_GS_SET_TRXDIR(1);
        store_image->trxdir.UI64[1] = SCE_GS_TRXDIR;

        flPs2StoreImageAdrs = load_ptr;
        flPs2StoreImageSize = trans_size / 16;
        *GS_CSR = 2;
        flPs2StoreImageOldIMR = *GS_IMR;
        *GS_IMR = -0x201;

        EnableIntc(0);
        dma_channel->chcr.TTE = 0;
        dma_channel->chcr.TIE = 0;
        FlushCache(WRITEBACK_DCACHE);
        sceDmaSend(dma_channel, (u32*)store_image);
        sceGsSyncPath(0, 0);

        while (flPs2StoreImageSize != 0) {
            // Do nothing
        }

        sceGsSyncPath(0, 0);
        *VIF1_STAT = 0;
        *GS_BUSDIR = 0;
        *GS_IMR = flPs2StoreImageOldIMR;
        *VIF1_FIFO = vif1_fifo;
        load_ptr += trans_size;
        y += wk_h;

        if (y + wk_h > h) {
            wk_h = h - y;
        }

        size -= trans_size;
    }
}

static s32 IntGsStoreImageHandler() {
    sceDmaChan* dma_channel;

    if (flPs2StoreImageSize != 0) {
        dma_channel = flPs2State.DmaChan[1];
        *VIF1_STAT = 0x800000;
        *GS_BUSDIR = 1;
        iDisableIntc(0);
        dma_channel->chcr.TTE = 1;
        dma_channel->chcr.TIE = 0;
        sceDmaRecvN(dma_channel, (u32*)flPs2StoreImageAdrs, flPs2StoreImageSize);
        flPs2StoreImageAdrs = 0;
        flPs2StoreImageSize = 0;
    }

    ExitHandler();
    return 0;
}

void flPS2DmaInitControl(FLPS2VIF1Control* dma_ptr, u32 queue_size, void* handler) {
    s32 lp0;
    dma_ptr->queue_size = queue_size;

    for (lp0 = 0; lp0 < 2; lp0++) {
        dma_ptr->dma_queue_handle[lp0] = flPS2GetSystemMemoryHandle(queue_size * 4, 1);
        dma_ptr->queue_ctr[lp0] = 0;
        dma_ptr->queue_ptr0[lp0] = 0;
        dma_ptr->queue_ptr1[lp0] = 0;
    }

    dma_ptr->handleID = AddDmacHandler(dma_ptr->channel_id, handler, 0);
    dma_ptr->dma_normal_mode_status = 0;
    EnableDmac(dma_ptr->channel_id);
    flPs2GsHandler = AddIntcHandler(0, &IntGsStoreImageHandler, 0);
}

s32 flPS2DmaAddQueue2(s32 type, uintptr_t data_adrs, uintptr_t endtag_adrs, FLPS2VIF1Control* dma_ptr) {
#if !defined(TARGET_PS2)
    // Return early because we don't need to handle DMA stuff on non-PS2 systems
    return 0;
#endif

    u32 dma_chcr;
    sceDmaChan* dma_channel;
    uintptr_t* dma_queue;
    s32 dma_index;
    uintptr_t* old_tag;
    u32 qwc;

    flDebugAQNum += 1;

    if (data_adrs == 0) {
        flPS2SystemError(0, "ERROR flPS2DmaAddQueue2 flps2dma.c");
    }

    if (type == 2) {
    block_1:
        dma_chcr = *dma_chcr_adrs[dma_ptr->channel_id];
        dma_channel = flPs2State.DmaChan[dma_ptr->channel_id];
        dma_index = flPs2State.SystemIndex;

        if (dma_ptr->queue_ctr[dma_index] <= dma_ptr->queue_size - 1) {
            dma_queue = (uintptr_t*)flPS2GetSystemBuffAdrs(dma_ptr->dma_queue_handle[dma_index]);
            dma_ptr->queue_ctr[dma_index] += 1;
            dma_queue[dma_ptr->queue_ptr0[dma_index]++] = data_adrs;

            if (dma_ptr->queue_ptr0[dma_index] == dma_ptr->queue_size) {
                dma_ptr->queue_ptr0[dma_index] = 0;
            }

            dma_ptr->old_queue_data = data_adrs;
            dma_ptr->old_endtag = endtag_adrs;

            return 0;
        }
    } else if (dma_ptr->old_queue_data && dma_ptr->old_endtag) {
        old_tag = (uintptr_t*)dma_ptr->old_endtag;

        switch ((dma_ptr->old_queue_data & 0xF0000000) >> 0x1C) {
        default:
        case 2:
            goto block_1;
            break;

        case 0:
        case 3:
        case 4:
        case 5:
            if ((*old_tag & 0x70000000) != 0x70000000) {
                dma_ptr;
                goto block_1;
            } else {
                qwc = *old_tag & 0xFFFF;
                *old_tag++ = (qwc + 0x20000000);
                *old_tag = data_adrs & 0xFFFFFFF;
            }
            goto block_2;

        case 1:
            if ((*old_tag & 0x70000000) != 0) {
                dma_ptr;
                goto block_1;
            } else {
                qwc = *old_tag & 0xFFFF;
                *old_tag = (qwc + 0x30000000);

                if (qwc == 0) {
                    flPS2SystemError(0, "ERROR flPS2DmaAddQueue2 flps2dma.c");
                }
            block_2:
                if (type == 1) {
                    dma_ptr->old_queue_data = 0;
                    dma_ptr->old_endtag = 0;
                } else {
                    dma_ptr->old_queue_data = data_adrs;
                    dma_ptr->old_endtag = endtag_adrs;
                }
                return 0;
            }
            break;
        }
    } else {
        goto block_1;
    }

    dma_ptr->old_queue_data = 0;
    dma_ptr->old_endtag = 0;
    return -1;
}

s32 flPS2DmaInterrupt(s32 ch) {
    FLPS2VIF1Control* dma_ptr;
    u32 dma_adrs;
    u32 dma_chcr;
    u32 type;
    u32* dma_queue;
    s32 dma_index;
    FLPS2StoreImageData* store_image;

    dma_ptr = &flPs2VIF1Control;

    if (ch == 1) {
        dma_chcr = *dma_chcr_adrs[dma_ptr->channel_id];
        if (!(dma_chcr & 0x100)) {
            if ((dma_chcr & 0xC) == 4) {
                if (dma_chcr & 0x80) {
                    if ((dma_chcr & 0x80000000)) {
                        goto block_11;
                    }
                }
            } else if (!(dma_chcr & 0xC) && (dma_ptr->dma_normal_mode_status & 1)) {
            block_11:
                flDebugDINum += 1;
                dma_index = flPs2State.SystemIndex ^ 1;
                dma_queue = (u32*)flPS2GetSystemBuffAdrs(dma_ptr->dma_queue_handle[dma_index]);
                dma_ptr->queue_ctr[dma_index] -= 1;
                dma_adrs = dma_queue[dma_ptr->queue_ptr1[dma_index]++];

                if (dma_ptr->queue_ptr1[dma_index] == dma_ptr->queue_size) {
                    dma_ptr->queue_ptr1[dma_index] = 0;
                }

                type = ((dma_adrs & 0xF0000000)) >> 0x1C;
                dma_adrs = dma_adrs & 0xFFFFFFF;

                switch (type) {
                case 2:
                    dma_ptr->dma_normal_mode_status = 0;
                    *VIF1_STAT = 0;
                    *GS_BUSDIR = 0;
                    *GS_IMR = flPs2StoreImageOldIMR;
                    *VIF1_FIFO = vif1_fifo;
                    break;
                }

                if (dma_ptr->queue_ptr0[dma_index] != dma_ptr->queue_ptr1[dma_index]) {
                    dma_adrs = dma_queue[dma_ptr->queue_ptr1[dma_index]];
                    dma_ptr->now_adrs = dma_adrs;
                    type = ((dma_adrs & 0xF0000000)) >> 0x1C;
                    dma_adrs = dma_adrs & 0xFFFFFFF;

                    if (dma_adrs != 0) {
                        switch (type) {
                        case 0:
                        case 1:
                        case 3:
                        case 4:
                        case 5:
                            flPs2State.DmaChan[dma_ptr->channel_id]->chcr.TTE = 1;
                            flPs2State.DmaChan[dma_ptr->channel_id]->chcr.TIE = 1;
                            sceDmaSend(flPs2State.DmaChan[dma_ptr->channel_id], (u32*)dma_adrs);
                            break;

                        case 2:
                            store_image = (FLPS2StoreImageData*)dma_adrs;
                            flPs2StoreImageAdrs = store_image->data.I32[0];
                            flPs2StoreImageSize = store_image->data.I32[1];
                            *GS_CSR = 2;
                            flPs2StoreImageOldIMR = *GS_IMR;
                            *GS_IMR = -0x201;
                            iEnableIntc(0);
                            dma_ptr->dma_normal_mode_status |= 1;
                            flPs2State.DmaChan[dma_ptr->channel_id]->chcr.TTE = 0;
                            flPs2State.DmaChan[dma_ptr->channel_id]->chcr.TIE = 0;
                            sceDmaSend(flPs2State.DmaChan[dma_ptr->channel_id], (u32*)dma_adrs);
                            break;
                        }
                    }
                } else {
                    flDebugEndRenderTime = *T0_COUNT;
                }
            }
        }
    }

    ExitHandler();
    return 0;
}

void flPS2DmaSend() {
    FLPS2VIF1Control* dma_ptr;
    u32* dma_queue;
    u32 data_adrs;
    u32 type;
    s32 dma_index;
    u32 dma_chcr;
    sceDmaChan* dma_channel;
    FLPS2StoreImageData* store_image;

    dma_ptr = &flPs2VIF1Control;
    dma_index = flPs2State.SystemIndex;
    flPs2State.SystemIndex = flPs2State.SystemIndex ^ 1;
    dma_ptr->old_queue_data = 0;
    dma_ptr->old_endtag = 0;

    if (dma_ptr->queue_ctr[dma_index] != 0) {
        dma_chcr = *dma_chcr_adrs[dma_ptr->channel_id];
        dma_channel = flPs2State.DmaChan[dma_ptr->channel_id];
        dma_queue = (u32*)flPS2GetSystemBuffAdrs(dma_ptr->dma_queue_handle[dma_index]);
        data_adrs = dma_queue[dma_ptr->queue_ptr1[dma_index]];
        dma_ptr->now_adrs = data_adrs;
        type = ((data_adrs & 0xF0000000)) >> 0x1C;
        data_adrs = data_adrs & 0xFFFFFFF;

        switch (type) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
            dma_channel->chcr.TTE = 1;
            dma_channel->chcr.TIE = 1;
            FlushCache(WRITEBACK_DCACHE);
            sceDmaSend(dma_channel, (u32*)data_adrs);
            break;

        case 2:
            store_image = (FLPS2StoreImageData*)data_adrs;
            flPs2StoreImageAdrs = store_image->data.UI32[0];
            flPs2StoreImageSize = store_image->data.UI32[1];
            *GS_CSR = 2;
            flPs2StoreImageOldIMR = *GS_IMR;
            *GS_IMR = -0x201;
            EnableIntc(0);
            dma_ptr->dma_normal_mode_status |= 1;
            dma_channel->chcr.TTE = 0;
            dma_channel->chcr.TIE = 0;
            FlushCache(WRITEBACK_DCACHE);
            sceDmaSend(dma_channel, (u32*)data_adrs);
            break;
        }
    }
}

s32 flPS2DmaWait() {
    s32 dma_index = flPs2State.SystemIndex ^ 1;

    while (flPs2VIF1Control.queue_ctr[dma_index] != 0) {
        // Do nothing
    }
    while (sceDmaSync(flPs2State.DmaChan[1], 1, 0) != 0) {
        // Do nothing
    }

    return 1;
}

s32 flPS2DmaTerminate() {
    s32 dma_index;
    FLPS2VIF1Control* dma_ptr = &flPs2VIF1Control;

    if ((dma_ptr->queue_ctr[0] == 0) && (dma_ptr->queue_ctr[1] == 0)) {
        return 0;
    }

    flPS2DmaWait();
    dma_index = flPs2State.SystemIndex;

    if (dma_ptr->queue_ctr[dma_index] == 0) {
        return 0;
    }

    flPS2DmaSend();
    flPS2DmaWait();
    flPS2SystemTmpBuffFlush();
    flPS2FlipCancelFlag = 2;

    return 1;
}
