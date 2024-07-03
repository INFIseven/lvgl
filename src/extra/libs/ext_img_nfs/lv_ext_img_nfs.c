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
#include "../../../../hpr_display/flash/include/GD25Q16C41.h"

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

static lv_res_t decoder_read_line_8bit(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc, lv_coord_t x, lv_coord_t y, lv_coord_t len, uint8_t *buf);
static lv_res_t decoder_read_line_16bit(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc, lv_coord_t x, lv_coord_t y, lv_coord_t len, uint8_t *buf);
static lv_res_t decoder_calculate_lines_8bit(const uint8_t *p_img_data, uint16_t img_w, uint16_t img_h);
static lv_res_t decoder_calculate_lines_16bit(const uint16_t *p_img_data, uint16_t img_w, uint16_t img_h);
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
    // lv_img_decoder_set_read_line_cb(dec, decoder_read_line_8bit);
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

    // lv_ext_img_nfs_t *img_header = (lv_ext_img_nfs_t *)src; /* Parse "LCD Image Converter" header */
    // if (img_header->cf != LV_IMG_CF_USER_ENCODED_0)
    // {
    //     return LV_RES_INV;
    // }

    // // lv_img_src_t src_type = lv_img_src_get_type(src);          /*Get the source type*/

    // // LV_IMG_SRC_VARIABLE check options
    // header->cf = LV_IMG_CF_USER_ENCODED_0;
    // switch (img_header->data_type)
    // {
    //     case RLE_DATA_8BIT:
    //         header->w = img_header->image_8bit.width;
    //         header->h = img_header->image_8bit.height;
    //         break;
    //     case RLE_DATA_16BIT:
    //         header->w = img_header->image_16bit.width;
    //         header->h = img_header->image_16bit.height;
    //         break;
    //     default:
    //         return LV_RES_INV;
    // }
    // header->always_zero = 0;

    return LV_RES_OK;
    // return LV_RES_INV;         /*If didn't succeeded earlier then it's an error*/
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
    // LV_UNUSED(decoder);

    // lv_ext_img_nfs_t *img_header = (lv_ext_img_nfs_t *)dsc->src;

    // if (img_header->cf != LV_IMG_CF_USER_ENCODED_0)
    // {
    //     return LV_RES_INV;
    // }
    // if (dsc->src_type == LV_IMG_SRC_VARIABLE)
    // {

    //     const uint8_t *p_img_data = (uint8_t *)img_header->image_8bit.data;
    //     if (p_img_data_store == p_img_data)
    //     {
    //         return LV_RES_OK;
    //     }
    //     p_img_data_store = (uint8_t *)p_img_data;

    //     switch (img_header->data_type)
    //     {
    //         case RLE_DATA_8BIT:
    //             lv_img_decoder_set_read_line_cb(decoder, decoder_read_line_8bit);
    //             decoder_calculate_lines_8bit(p_img_data, img_header->image_8bit.width, img_header->image_8bit.height);
    //             return LV_RES_OK;
    //             break;
    //         case RLE_DATA_16BIT:
    //             lv_img_decoder_set_read_line_cb(decoder, decoder_read_line_16bit);
    //             decoder_calculate_lines_16bit((const uint16_t *)p_img_data, img_header->image_16bit.width, img_header->image_16bit.height);
    //             return LV_RES_OK;
    //             break;
    //         default:
    //             return LV_RES_INV;
    //     }
    // }
    // else
    // {
    //     return LV_RES_INV;
    // }

    return LV_RES_INV; /*If not returned earlier then it failed*/
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
