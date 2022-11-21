#ifndef MP3EDIT_H
#define MP3EDIT_H

#include "types.h" // Contains user defined types

typedef struct _TagEditInfo // Structure to store tagging information.
{
    /* Mp3 info */
    char *mp3_fname;
    FILE *fptr_mp3_old;
    FILE *fptr_mp3_new;
    /* Mp3 tag data info */
    uint info_size;
    uint edit_operation;
    char *edit_data;
    char *edited_title_name;
    char tag[5];
    char tag_data[100];
    /* Mp3 tag position info */
    uint title_pos, artist_pos, album_pos,
         composer_pos, year_pos, genre_pos;
} TagEditInfo;

/* Read and validate Edit args from argv */
Status read_and_validate_edit_args(char *argv[], TagEditInfo *tageInfo);

/* Read and validate Edit args options from argv */
EditOperationType check_edit_operation_type(char *argv[]);

/* Perform the editing */
Status do_editing(TagEditInfo *tageInfo);

/* Get File pointer for mp3 files */
Status open_mp3_files(TagEditInfo *tageInfo);

/* Read the positions of each tag */
Status tag_pos_reader(TagEditInfo *tageInfo);

/* Store the positions of each tag */
Status tag_pos_storage(TagEditInfo *tageInfo);

/* finding tag position and returning it */
uint tag_position_finder(TagEditInfo *tageInfo);

/* Edit the mp3 file according to the option */
Status edit_operation(TagEditInfo *tageInfo);

/* Show the edited tag data */
Status edited_tag_data_view(TagEditInfo *tageInfo);

/* Print n number of dash*/
void dash(int n);

#endif
