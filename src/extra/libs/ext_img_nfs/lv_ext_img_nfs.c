/**
 * @file lv_ext_img_nfs.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"

#if LV_USE_EXT_IMG_NFS
#include "lv_ext_img_nfs.h"
#include <string.h>
#include <stdio.h>
#ifndef DEVICE_SIMULATOR
#include "../../../../hpr_display/flash/include/GD25Q16C41.h"
#endif
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_res_t decoder_info(lv_img_decoder_t *decoder, const void *src, lv_img_header_t *header);
static lv_res_t decoder_open(lv_img_decoder_t *dec, lv_img_decoder_dsc_t *dsc);
static lv_res_t decoder_read_line(lv_img_decoder_t     *decoder,
                  lv_img_decoder_dsc_t *dsc,
                  lv_coord_t            x,
                  lv_coord_t            y,
                  lv_coord_t            len,
                  uint8_t              *buf);
static void     decoder_close(lv_img_decoder_t *dec, lv_img_decoder_dsc_t *dsc);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
static inline void
memset16(void *m, uint16_t val, size_t count)
{
    uint16_t *buf = m;
    while (count--)
    {
        *buf++ = val;
    }
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void
lv_ext_img_nfs_init(void)
{
    lv_img_decoder_t *dec = lv_img_decoder_create();
    lv_img_decoder_set_info_cb(dec, decoder_info);
    lv_img_decoder_set_open_cb(dec, decoder_open);
    lv_img_decoder_set_read_line_cb(dec, decoder_read_line);
    lv_img_decoder_set_close_cb(dec, decoder_close);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Get info about a PNG image
 * @param src can be file name or pointer to a C array
 * @param header store the info here
 * @return LV_RES_OK: no error; LV_RES_INV: can't get the info
 */
static lv_res_t
decoder_info(lv_img_decoder_t *decoder, const void *src, lv_img_header_t *header)
{
    LV_UNUSED(decoder);

    lv_ext_img_nfs_t *img_header = (lv_ext_img_nfs_t *)src; /* Parse "LCD Image Converter" header */
    //printf("img_header->cf: %d %d %d\n", img_header->cf, LV_IMG_CF_USER_ENCODED_1, LV_IMG_CF_EXT_IMG_NFS);
    if (img_header->header.cf != LV_IMG_CF_EXT_IMG_NFS)
    {
        return LV_RES_INV;
    }
    uint32_t address = *(uint32_t*)img_header->data;
#ifndef DEVICE_SIMULATOR
    ext_flash_read(address, (uint8_t *)header, sizeof(lv_img_header_t));
#else
    memcpy(header, &data_test[0], sizeof(lv_img_header_t));
#endif
    header->cf = LV_IMG_CF_EXT_IMG_NFS;
    // printf("header->cf: %d\n", header->cf);
    // printf("header->w: %d\n", header->w);
    // printf("header->h: %d\n", header->h);
    // printf("header->always_zero: %d\n", header->always_zero);
    if ((header->cf == LV_IMG_CF_EXT_IMG_NFS) && 
        (header->w != 0) && 
        (header->h != 0) && 
        (header->always_zero == 0) &&
        (header->w != 0x7ff) && 
        (header->h != 0x7ff))
    {
        img_header->header.w = header->w;
        img_header->header.h = header->h;
        return LV_RES_OK;
    }
    // printf("decoder error!");
    return LV_RES_INV; /*If didn't succeeded earlier then it's an error*/
}

/**
 * Open a PNG image and return the decided image
 * @param decoder pointer to the decoder where this function belongs
 * @param dsc pointer to a descriptor which describes this decoding session
 * @return LV_RES_OK: no error; LV_RES_INV: can't get the info
 */
static lv_res_t
decoder_open(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc)
{
    LV_UNUSED(decoder);
    //LV_UNUSED(dsc);
    
    lv_ext_img_nfs_t *img_header = (lv_ext_img_nfs_t *)dsc->src;

    return LV_RES_OK;
}

static lv_res_t
decoder_read_line(lv_img_decoder_t     *decoder,
                  lv_img_decoder_dsc_t *dsc,
                  lv_coord_t            x,
                  lv_coord_t            y,
                  lv_coord_t            len,
                  uint8_t              *buf)
{
    LV_UNUSED(decoder);
#ifndef DEVICE_SIMULATOR
    lv_ext_img_nfs_t *img_header = (lv_ext_img_nfs_t *)dsc->src;
    uint32_t offset = 4 + (((y*img_header->header.w)+x) * 2);
    uint32_t address = *(uint32_t*)img_header->data;
    ext_flash_read(address + offset, (uint8_t *)buf, len*2);
#else
     memcpy(buf, &data_test[4 + (((y*img_header->header.w)+x) * 2)], len*2);
#endif
   

    return LV_RES_OK;
}

/**
 * Free the allocated resources
 */
static void
decoder_close(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc)
{
    LV_UNUSED(decoder);
    // lv_mem_free(p_img_y_ind);
    // lv_mem_free(p_img_y_seq_off);
}

#endif /*LV_USE_EXT_IMG_NFS*/
