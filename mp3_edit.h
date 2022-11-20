#ifndef MP3EDIT_H
#define MP3EDIT_H

#include "types.h" // Contains user defined types

typedef struct _TagEditInfo // Structure to store tagging information.
{
    /* Mp3 info */
    char *mp3_fname;
    FILE *fptr_mp3_old;
    FILE *fptr_mp3_new;
    uint info_size;
    uint edit_operation;
    char *edit_data;
    char *edited_title_name;
    char tag[5];
    char tag_data[100];
    uint title_pos, artist_pos, album_pos,
         composer_pos, year_pos, genre_pos;
} TagEditInfo;

/* Read and validate Edit args from argv */
Status read_and_validate_edit_args(char *argv[], TagEditInfo *tageInfo);

EditOperationType check_edit_operation_type(char *argv[]);

/* Perform the editing */
Status do_editing(TagEditInfo *tageInfo);

/* Get File pointer for mp3 file */
Status open_mp3_files(TagEditInfo *tageInfo);

Status tag_pos_reader(TagEditInfo *tageInfo);

Status tag_pos_storage(TagEditInfo *tageInfo);

uint tag_position_finder(TagEditInfo *tageInfo);

Status edit_operation(TagEditInfo *tageInfo);

Status edited_tag_data_view(TagEditInfo *tageInfo);

void dash(int n);
#endif
