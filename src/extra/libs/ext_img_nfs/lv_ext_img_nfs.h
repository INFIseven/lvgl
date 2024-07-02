/**
 * @file lv_ext_img_nfs.h
 *
 */

#ifndef LV_EXT_IMG_NFS_H
#define LV_EXT_IMG_NFS_H

#define LV_USE_EXT_IMG_NFS 1
#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
// #include "../../../lv_conf_internal.h"
#if LV_USE_EXT_IMG_NFS

/*********************
 *      DEFINES
 *********************/
#define LV_IMG_CF_EXT_IMG_NFS LV_IMG_CF_USER_ENCODED_1

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
    uint32_t cf : 5;          /*Color format: See `lv_img_color_format_t`*/
    uint32_t always_zero : 3; /*It the upper bits of the first byte. Always zero to look like a
                             non-printable character*/
    uint32_t flash_addr;
} lv_ext_img_nfs_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_ext_img_nfs_init(void);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_BMP*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_EXT_IMG_NFS_H*/
