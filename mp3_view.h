#ifndef MP3VIEW_H
#define MP3VIEW_H

#include "types.h" // Contains user defined types

typedef struct _TagViewInfo // Structure to store tagging information.
{
    /* Mp3 info */
    char *mp3_fname;
    FILE *fptr_mp3;
    uint info_size;
    uint bits_per_pixel;
    char image_data[100];
    char title[100], artist[100], album[100],
         Composer[100], year[100], genre[100];
} TagViewInfo;

/* Read and validate View args from argv */
Status read_and_validate_view_args(char *argv[], TagViewInfo *tagvInfo);

/* Perform the viewing */
Status do_viewing(TagViewInfo *tagvInfo);

/* Get File pointer for mp3 file */
Status open_mp3_file(TagViewInfo *tagvInfo);
#endif
