#include <stdio.h>
#include <string.h>
#include "mp3_edit.h"
#include "types.h"
//#include "common.h"

/* Function Definitions for Editing Tags */

/* Read and validate command line argument
 * Input: Arguments with File name info
 * Output: File names stored in tags Info
 * Return: e_success or e_failure
 */

Status read_and_validate_edit_args(char *argv[], TagEditInfo *tageInfo)
{
    if(strcmp(strstr(argv[2], "."), ".mp3") == 0)
    {
        tageInfo ->mp3_fname = argv[2];
    }
    else
    {
        printf("ERROR : Source audio file %s extension should be .mp3\n", argv[2]);
        return e_failure;
    }

    if(((strcmp(argv[3], "-a")) == 0) || ((strcmp(argv[3], "-t")) == 0) || ((strcmp(argv[3], "-A")) == 0) || ((strcmp(argv[3], "-y")) == 0) || ((strcmp(argv[3], "-g")) == 0) || ((strcmp(argv[3], "-c")) == 0))
    {
        tageInfo->edit_operation = check_edit_operation_type(argv);
    }
    else
    {
        printf("ERROR : Invalid edit option selected\n");
        return e_failure;
    }

    tageInfo->edit_data = argv[4];

    return e_success;
}


EditOperationType check_edit_operation_type(char *argv[])
{
    if((strcmp(argv[3], "-a")) == 0)
    {
        return e_edit_artist;
    }
    else if((strcmp(argv[3], "-t")) == 0)
    {
        return e_edit_title;
    }
    else if((strcmp(argv[3], "-A")) == 0)
    {
        return e_edit_album;
    }
    else if((strcmp(argv[3], "-y")) == 0)
    {
        return e_edit_year;
    }
    else if((strcmp(argv[3], "-g")) == 0)
    {
        return e_edit_genre;
    }
    else if((strcmp(argv[3], "-c")) == 0)
    {
        return e_edit_composer;
    }
}

/* Encoding the secret file data to stego image
 * Input: FILE info of image, secret file and stego image
 * Output: Encodes the data in secret to stego image
 * Return: e_success or e_failure;
 */
Status do_editing(TagEditInfo *tageInfo)
{
    // Open files needed for viewing tags
    if(open_mp3_files(tageInfo) == e_failure)
    {
        printf("ERROR : Opening mp3 file is failure\n");
        return e_failure;
    }
    char str[5];
    fread(&str, sizeof(char), 4, tageInfo->fptr_mp3_old);
    str[4] = '\0';
    if(strncmp(str, "ID3", 3) == 0)
    {
        fseek(tageInfo->fptr_mp3_old, 10, SEEK_SET);
        for(uint i=0;i<8;i++)
        {
            tag_pos_reader(tageInfo);
        }
        edit_operation(tageInfo);
        //edited_tag_data_view(tageInfo);
        return e_success;
    }
    else
    {
        printf("ERROR : %s is not an ID3V2 type mp3 file\n", tageInfo->mp3_fname);
        return e_failure;
    }
}

/*
 * Get File pointer for mp3 file
 * Inputs: File info
 * Output: FILE pointer for mp3 file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_mp3_files(TagEditInfo *tageInfo)
{
    //printf("INFO : Opening mp3 files\n");
    // Mp3 file
    tageInfo->fptr_mp3_old = fopen(tageInfo->mp3_fname, "rb");
    // Do Error handling
    if (tageInfo->fptr_mp3_old == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR : Unable to open file %s\n", tageInfo->mp3_fname);
    	return e_failure;
    }

    // Mp3 file
    tageInfo->fptr_mp3_new = fopen("new.mp3", "wb+");
    // Do Error handling
    if (tageInfo->fptr_mp3_new == NULL)
    {
    	perror("fopen");
    	return e_failure;
    }
    return e_success;
}

Status tag_pos_reader(TagEditInfo *tageInfo)
{
    int size=0, flag = 0, m=0;
    char ch;
    if(fgetc(tageInfo->fptr_mp3_old) == 'T')
    {
        fseek(tageInfo->fptr_mp3_old, -1, SEEK_CUR);
        fread(tageInfo->tag, sizeof(char), 4, tageInfo->fptr_mp3_old);
        tageInfo->tag[4]='\0';
        //printf("%s\n", tageInfo->tag);
        tag_pos_storage(tageInfo);
        fseek(tageInfo->fptr_mp3_old, 3, SEEK_CUR);
        fread(&size, sizeof(char), 1, tageInfo->fptr_mp3_old);
        fseek(tageInfo->fptr_mp3_old, 2, SEEK_CUR);
        fread(&flag, sizeof(char), 1, tageInfo->fptr_mp3_old);
        if(flag == 0)
        {
            fread(tageInfo->tag_data, sizeof(char), size-1, tageInfo->fptr_mp3_old);
        }
        else
        {
            fseek(tageInfo->fptr_mp3_old, 1, SEEK_CUR);
            m=(size/2);
            for(uint j=0;j<m;j++)
            {
                fseek(tageInfo->fptr_mp3_old, 1, SEEK_CUR);
                fread(&ch, sizeof(char), 1, tageInfo->fptr_mp3_old);
            }
            fseek(tageInfo->fptr_mp3_old, -1, SEEK_CUR);
        }
    }
    return e_success;
}


Status tag_pos_storage(TagEditInfo *tageInfo)
{
    if(strcmp(tageInfo->tag, "TIT2") == 0)
    {
        tageInfo->title_pos = ftell(tageInfo->fptr_mp3_old);
    }
    else if(strcmp(tageInfo->tag, "TPE1") == 0)
    {
        tageInfo->artist_pos = ftell(tageInfo->fptr_mp3_old);
    }
    else if(strcmp(tageInfo->tag, "TALB") == 0)
    {
        tageInfo->album_pos = ftell(tageInfo->fptr_mp3_old);
    }
    else if(strcmp(tageInfo->tag, "TYER") == 0)
    {
        tageInfo->year_pos = ftell(tageInfo->fptr_mp3_old);
    }
    else if(strcmp(tageInfo->tag, "TCON") == 0)
    {
        tageInfo->genre_pos = ftell(tageInfo->fptr_mp3_old);
    }
    else if(strcmp(tageInfo->tag, "TCOM") == 0)
    {
        tageInfo->composer_pos = ftell(tageInfo->fptr_mp3_old);
    }
    return e_success;
}

uint tag_position_finder(TagEditInfo *tageInfo)
{
    if(tageInfo->edit_operation == e_edit_title)
    {
        tageInfo->edited_title_name = "TITLE";
        return tageInfo->title_pos;
    }
    else if(tageInfo->edit_operation == e_edit_artist)
    {
        tageInfo->edited_title_name = "ARTIST";
        return tageInfo->artist_pos;
    }
    else if(tageInfo->edit_operation == e_edit_album)
    {
        tageInfo->edited_title_name = "ALBUM";
        return tageInfo->album_pos;
    }
    else if(tageInfo->edit_operation == e_edit_year)
    {
        tageInfo->edited_title_name = "YEAR";
        return tageInfo->year_pos;
    }
    else if(tageInfo->edit_operation == e_edit_genre)
    {
        tageInfo->edited_title_name = "MUSIC";
        return tageInfo->genre_pos;
    }
    else if(tageInfo->edit_operation == e_edit_composer)
    {
        tageInfo->edited_title_name = "COMPOSER";
        return tageInfo->composer_pos;
    }
}

Status edit_operation(TagEditInfo *tageInfo)
{
    uint a, n, m;
    unsigned char pad1[3]={0x00, 0x00, 0x00};
    char ch;
    uint pos = tag_position_finder(tageInfo) + 3;

    rewind(tageInfo->fptr_mp3_old);
    for(int i = 0; i< pos;i++)
    {
        fread(&ch, sizeof(char), 1, tageInfo->fptr_mp3_old);
        fwrite(&ch, sizeof(char), 1, tageInfo->fptr_mp3_new);
    }
    fread(&ch, sizeof(char), 1, tageInfo->fptr_mp3_old);
    a = (int)ch;
    fseek(tageInfo->fptr_mp3_old, a+2, SEEK_CUR);
    n = strlen(tageInfo->edit_data);
    m = n + 1;
    fwrite(&m, sizeof(char), 1, tageInfo->fptr_mp3_new);
    fwrite(pad1, sizeof(pad1), 1, tageInfo->fptr_mp3_new);
    fwrite(tageInfo->edit_data, sizeof(char), n, tageInfo->fptr_mp3_new);

    while(fread(&ch, sizeof(char), 1, tageInfo->fptr_mp3_old) > 0)
        fwrite(&ch, sizeof(char), 1, tageInfo->fptr_mp3_new);

    return e_success;
}

Status edited_tag_data_view(TagEditInfo *tageInfo)
{
    int size;
    char data[50];
    uint pos = tag_position_finder(tageInfo) + 3;
    fseek(tageInfo->fptr_mp3_new, pos, SEEK_SET);
    fread(&size, sizeof(char), 1, tageInfo->fptr_mp3_new);
    fseek(tageInfo->fptr_mp3_new, 3, SEEK_CUR);
    fread(&data, sizeof(char), size-1, tageInfo->fptr_mp3_new);
    dash(15); printf(" SELECTED EDIT OPTION "); dash(15); printf("\n");
    dash(15); printf(" CHANGE THE %s ", tageInfo->edited_title_name); dash(15); printf("\n");
    dash(63); printf("\n\n");
    printf("%-10s  :       %s\n\n", tageInfo->edited_title_name, data);
    dash(63); printf("\n\n");
    return e_success;
}
