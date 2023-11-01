/**
 * @file lv_bmp.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"
#define LV_USE_RLE 1

#if LV_USE_RLE
#include "lv_rle.h"
#include <string.h>

/*********************
 *      DEFINES
 *********************/
#define LCD_H 320
#define LCD_W 170

#if (LCD_W > 255)

#endif
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_res_t decoder_info(lv_img_decoder_t *decoder, const void *src, lv_img_header_t *header);
static lv_res_t decoder_open(lv_img_decoder_t *dec, lv_img_decoder_dsc_t *dsc);

static lv_res_t decoder_read_line_8bit(lv_img_decoder_t     *decoder,
                                  lv_img_decoder_dsc_t *dsc,
                                  lv_coord_t            x,
                                  lv_coord_t            y,
                                  lv_coord_t            len,
                                  uint8_t              *buf);
static lv_res_t decoder_read_line_16bit(lv_img_decoder_t     *decoder,
                                  lv_img_decoder_dsc_t *dsc,
                                  lv_coord_t            x,
                                  lv_coord_t            y,
                                  lv_coord_t            len,
                                  uint8_t              *buf);
static lv_res_t decoder_calculate_lines_8bit(const uint8_t *p_img_data, uint16_t img_w, uint16_t img_h);
static lv_res_t decoder_calculate_lines_16bit(const uint16_t *p_img_data, uint16_t img_w, uint16_t img_h);
static void decoder_close(lv_img_decoder_t *dec, lv_img_decoder_dsc_t *dsc);

/**********************
 *  STATIC VARIABLES
 **********************/
#if 0
static uint16_t* p_img_y_ind;     /* Pointer to array where the index of each y line will be stored */
static int16_t* p_img_y_seq_off; /* Pointer to array where the sequence offset of each y axis will be stored */
#endif
static uint16_t
    p_img_y_ind[LCD_H]; /* Pointer to array where the index of each y line will be stored */
static int8_t p_img_y_seq_off_8bit
    [LCD_H]; /* Pointer to array where the sequence offset of each y axis will be stored */
    static int16_t p_img_y_seq_off_16bit
    [LCD_H]; /* Pointer to array where the sequence offset of each y axis will be stored */
static uint8_t *p_img_data_store;

/**********************
 *      MACROS
 **********************/
static inline void memset16(void* m, uint16_t val, size_t count)
{
	uint16_t* buf = m;
	while (count--) {
		*buf++ = val;
	}
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void
lv_rle_init(void)
{
    lv_img_decoder_t *dec = lv_img_decoder_create();
    lv_img_decoder_set_info_cb(dec, decoder_info);
    lv_img_decoder_set_open_cb(dec, decoder_open);
    //lv_img_decoder_set_read_line_cb(dec, decoder_read_line_8bit);
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

    lv_rle_t *img_header = (lv_rle_t *)src; /* Parse "LCD Image Converter" header */
    if (img_header->cf != LV_IMG_CF_USER_ENCODED_0)
        return LV_RES_INV;

    //lv_img_src_t src_type = lv_img_src_get_type(src);          /*Get the source type*/

    // LV_IMG_SRC_VARIABLE check options
    header->cf          = LV_IMG_CF_USER_ENCODED_0;
    switch (img_header->data_type)
    {
        case RLE_DATA_8BIT:
            header->w           = img_header->image_8bit.width;
            header->h           = img_header->image_8bit.height;
            break;
        case RLE_DATA_16BIT:
            header->w           = img_header->image_16bit.width;
            header->h           = img_header->image_16bit.height;
            break;
        default:
            return LV_RES_INV;
    }
    header->always_zero = 0;

    return LV_RES_OK;
    //return LV_RES_INV;         /*If didn't succeeded earlier then it's an error*/
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
    //LV_UNUSED(decoder);

    lv_rle_t *img_header = (lv_rle_t *)dsc->src;

    if (img_header->cf != LV_IMG_CF_USER_ENCODED_0) {
        return LV_RES_INV;
    }
    if (dsc->src_type == LV_IMG_SRC_VARIABLE)
    {
        
        const uint8_t *p_img_data = (uint8_t *)img_header->image_8bit.data;
        if (p_img_data_store == p_img_data)
        {
            return LV_RES_OK;
        }
        p_img_data_store         = (uint8_t *)p_img_data;

        switch (img_header->data_type)
        {
            case RLE_DATA_8BIT:
                lv_img_decoder_set_read_line_cb(decoder, decoder_read_line_8bit);
                decoder_calculate_lines_8bit(p_img_data, img_header->image_8bit.width, img_header->image_8bit.height);
                return LV_RES_OK;
                break;
            case RLE_DATA_16BIT:
                lv_img_decoder_set_read_line_cb(decoder, decoder_read_line_16bit);
                decoder_calculate_lines_16bit((const uint16_t *)p_img_data, img_header->image_16bit.width, img_header->image_16bit.height);
                return LV_RES_OK;
                break;
            default:
                return LV_RES_INV;
        }
    }
    else
    {
        return LV_RES_INV;
    }


    return LV_RES_INV; /*If not returned earlier then it failed*/
}

static lv_res_t
decoder_calculate_lines_8bit(const uint8_t *p_img_data, uint16_t img_w, uint16_t img_h)
{
    int8_t  sequence        = 0;
    int8_t  nonsequence     = 0;
    uint16_t cur_ind         = 0;
    uint32_t pix_cnt         = 0;
    uint16_t cnt_ind         = 0;
    uint8_t  last_state      = 1;
    p_img_y_ind[cnt_ind]     = 1;
    p_img_y_seq_off_8bit[cnt_ind] = p_img_data[0];
    cnt_ind++;

    do
    {
        if ((0 == sequence) && (0 == nonsequence))
        {
            sequence = (int8_t)p_img_data[cur_ind++];
            if (sequence < 0)
            {
                nonsequence = -sequence;
                sequence    = 0;
                last_state  = 0;
            }
            else
            {
                last_state = 1;
            }
        }
        if (sequence > 0)
        {
            sequence--;
            if (0 == sequence)
            {
                cur_ind++;
            }
        }
        if (nonsequence > 0)
        {
            cur_ind++;
            nonsequence--;
        }
        pix_cnt++;
        if (pix_cnt >= img_w)
        {
            p_img_y_ind[cnt_ind]     = cur_ind;
            p_img_y_seq_off_8bit[cnt_ind] = last_state ? sequence : -nonsequence;
            cnt_ind++;
            pix_cnt = 0;
        }
    } while (cnt_ind < img_h);

    return LV_RES_OK;
}

static lv_res_t
decoder_calculate_lines_16bit(const uint16_t *p_img_data, uint16_t img_w, uint16_t img_h)
{
    int16_t  sequence        = 0;
    int16_t  nonsequence     = 0;
    uint16_t cur_ind         = 0;
    uint32_t pix_cnt         = 0;
    uint16_t cnt_ind         = 0;
    uint8_t  last_state      = 1;
    p_img_y_ind[cnt_ind]     = 1;
    p_img_y_seq_off_16bit[cnt_ind] = p_img_data[0];
    cnt_ind++;

    do
    {
        if ((0 == sequence) && (0 == nonsequence))
        {
            sequence = (int16_t)p_img_data[cur_ind++];
            if (sequence < 0)
            {
                nonsequence = -sequence;
                sequence    = 0;
                last_state  = 0;
            }
            else
            {
                last_state = 1;
            }
        }
        if (sequence > 0)
        {
            sequence--;
            if (0 == sequence)
            {
                cur_ind++;
            }
        }
        if (nonsequence > 0)
        {
            cur_ind++;
            nonsequence--;
        }
        pix_cnt++;
        if (pix_cnt >= img_w)
        {
            p_img_y_ind[cnt_ind]     = cur_ind;
            p_img_y_seq_off_16bit[cnt_ind] = last_state ? sequence : -nonsequence;
            cnt_ind++;
            pix_cnt = 0;
        }
    } while (cnt_ind < img_h);

    return LV_RES_OK;
}

static lv_res_t
decoder_read_line_8bit(lv_img_decoder_t     *decoder,
                  lv_img_decoder_dsc_t *dsc,
                  lv_coord_t            x,
                  lv_coord_t            y,
                  lv_coord_t            len,
                  uint8_t              *buf)
{
    LV_UNUSED(decoder);

    uint8_t *p_img_data = (uint8_t *)&((lv_rle_t *)dsc->src)->image_8bit.data[p_img_y_ind[y]];
    lv_color16_t col_bck = ((lv_rle_t *)dsc->src)->image_8bit.color_bck;
    lv_color16_t col_obj = ((lv_rle_t *)dsc->src)->image_8bit.color_obj;
    
    int8_t   sequence    = p_img_y_seq_off_8bit[y];
    lv_color_t color;

    uint16_t cnt = 0;
    uint16_t diff;
    if (sequence > 0) {
        color = lv_color_mix(col_obj, col_bck, (uint8_t)*p_img_data);
        diff = len - cnt;
        sequence = (sequence > diff) ? diff : sequence;
        memset16(buf, color.full, sequence);
        buf += (2 * sequence);
        p_img_data++;
        cnt += sequence;
        sequence = 0;
    }

    //for (uint16_t cnt = 0; cnt < len; cnt++)
    while (cnt < len)
    {
        if (sequence == 0)
        {
            sequence = (int8_t)*p_img_data++;
            if (sequence > 0) {
                color = lv_color_mix(col_obj, col_bck, (uint8_t)*p_img_data);
                diff = len - cnt;
                sequence = (sequence > diff) ? diff : sequence;
                memset16(buf, color.full, sequence);
                buf += (2 * sequence);
                p_img_data++;
                cnt += sequence;
                sequence = 0;
            }
        } else {
            *((uint16_t*)buf) = lv_color_mix(col_obj, col_bck,(uint8_t)*p_img_data).full;
            sequence++;
            p_img_data++;
            cnt++;
            buf += 2;
        }
    }
    return LV_RES_OK;
}

static lv_res_t
decoder_read_line_16bit(lv_img_decoder_t     *decoder,
                  lv_img_decoder_dsc_t *dsc,
                  lv_coord_t            x,
                  lv_coord_t            y,
                  lv_coord_t            len,
                  uint8_t              *buf)
{
    LV_UNUSED(decoder);
    uint16_t *p_img_data = (uint16_t *)&((lv_rle_t *)dsc->src)->image_16bit.data[p_img_y_ind[y]];
    int16_t   sequence    = p_img_y_seq_off_16bit[y];

    uint16_t cnt = 0;
    uint16_t diff;
    if (sequence == 0)
    {
        sequence = (int16_t)*p_img_data++;
    }

    while (cnt < len)
    {
        if (sequence > 0) {
            diff = len - cnt;
            sequence = (sequence > diff) ? diff : sequence;
            memset16(buf, *p_img_data, sequence);
            p_img_data++;
        } else {
            diff = len - cnt;
            sequence = -sequence;
            sequence = (sequence > diff) ? diff : sequence;
            memcpy(buf, (uint8_t*)p_img_data, (2 * sequence));
            p_img_data += sequence;
        }
        buf += (2 * sequence);
        cnt += sequence;
        sequence = (int16_t)*p_img_data++;
    }
    return LV_RES_OK;
}
/**
 * Free the allocated resources
 */
static void
decoder_close(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc)
{
    LV_UNUSED(decoder);
    //lv_mem_free(p_img_y_ind);
    //lv_mem_free(p_img_y_seq_off);
}

#endif /*LV_USE_RLE*/
