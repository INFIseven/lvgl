/**
 * @file lv_ext_img_nfs.h
 *
 */

#ifndef LV_EXT_IMG_NFS_H
#define LV_EXT_IMG_NFS_H

#ifdef __cplusplus
extern "C" {
#endif
#define LV_USE_EXT_IMG_NFS 1
/*********************
 *      INCLUDES
 *********************/
#include "../../../lv_conf_internal.h"
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
    lv_img_header_t header; /**< A header describing the basics of the image*/
    uint32_t data_size;     /**< Size of the image in bytes*/
    const uint8_t * data;   /**< Pointer to the data of the image*/
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
