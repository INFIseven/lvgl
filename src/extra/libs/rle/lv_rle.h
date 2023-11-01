/**
 * @file lv_rle.h
 *
 */

#ifndef LV_RLE_H
#define LV_RLE_H

#define LV_USE_RLE 1
#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
//#include "../../../lv_conf_internal.h"
#if LV_USE_RLE

    /*********************
 *      DEFINES
 *********************/

    /**********************
 *      TYPEDEFS
 **********************/
 typedef enum {
    RLE_DATA_8BIT = 0,
    RLE_DATA_16BIT,
 } rle_data_t;
  
typedef struct
{
    const uint8_t *data;
    lv_color_t color_bck;
    lv_color_t color_obj;
    uint16_t        width;
    uint16_t        height;
} rle_img_8bit_t;

typedef struct
{
    const uint16_t *data;
    uint16_t        width;
    uint16_t        height;
} rle_img_16bit_t;

typedef struct
{
    uint32_t cf          : 5; /*Color format: See `lv_img_color_format_t`*/
    uint32_t always_zero : 3; /*It the upper bits of the first byte. Always zero to look like a
                             non-printable character*/
    rle_data_t data_type;
    union {
        rle_img_8bit_t image_8bit;
        rle_img_16bit_t image_16bit;
    };
    
} lv_rle_t;

    /**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_rle_init(void);

    /**********************
 *      MACROS
 **********************/

#endif /*LV_USE_BMP*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_RLE_H*/
