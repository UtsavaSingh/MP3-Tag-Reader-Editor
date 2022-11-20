#include <stdio.h>
#include <string.h>
#include "mp3_view.h"
#include "types.h"
//#include "common.h"

/* Function Definitions for Viewing Tags */

/* Read and validate command line argument
 * Input: Arguments with File name info
 * Output: File names stored in tags Info
 * Return: e_success or e_failure
 */

Status read_and_validate_view_args(char *argv[], TagViewInfo *tagvInfo)
{
    if(strcmp(strstr(argv[2], "."), ".mp3") == 0)
    {
        tagvInfo ->mp3_fname = argv[2];
        return e_success;
    }
    else
    {
        printf("ERROR : Source audio file %s extension should be .mp3\n", argv[2]);
        return e_failure;
    }
}

/* Encoding the secret file data to stego image
 * Input: FILE info of image, secret file and stego image
 * Output: Encodes the data in secret to stego image
 * Return: e_success or e_failure;
 */
Status do_viewing(TagViewInfo *tagvInfo)
{
    // Open files needed for viewing tags
    if(open_mp3_file(tagvInfo) == e_failure)
    {
        printf("ERROR : Opening mp3 file is failure\n");
        return e_failure;
    }
    char str[5];
    fread(&str, sizeof(char), 4, tagvInfo->fptr_mp3);
    str[4] = '\0';
    if(strncmp(str, "ID3", 3) == 0)
    {
        fseek(tagvInfo->fptr_mp3, 10, SEEK_SET);
        for(uint i=0;i<8;i++)
        {
            tag_reader(tagvInfo);
            tag_data_storage(tagvInfo);
        }
        tag_data_view(tagvInfo);
        return e_success;
    }
    else
    {
        printf("ERROR : %s is not an ID3V2 type mp3 file\n", tagvInfo->mp3_fname);
        return e_failure;
    }
}

/*
 * Get File pointer for mp3 file
 * Inputs: File info
 * Output: FILE pointer for mp3 file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_mp3_file(TagViewInfo *tagvInfo)
{
    //printf("INFO : Opening mp3 file\n");
    // Mp3 file
    tagvInfo->fptr_mp3 = fopen(tagvInfo->mp3_fname, "rb");
    // Do Error handling
    if (tagvInfo->fptr_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR : Unable to open file %s\n", tagvInfo->mp3_fname);

    	return e_failure;
    }
    else
    {
        //printf("INFO : Opened %s\n", tagvInfo -> mp3_fname);
        return e_success;
    }
}

Status tag_reader(TagViewInfo *tagvInfo)
{
    int size=0, flag = 0, m=0;
    char ch;
    if(fgetc(tagvInfo->fptr_mp3) == 'T')
    {
        fseek(tagvInfo->fptr_mp3, -1, SEEK_CUR);
        fread(tagvInfo->tag, sizeof(char), 4, tagvInfo->fptr_mp3);
        tagvInfo->tag[4]='\0';
        //printf("%s\n", tagvInfo->tag);
        fseek(tagvInfo->fptr_mp3, 3, SEEK_CUR);
        fread(&size, sizeof(char), 1, tagvInfo->fptr_mp3);
        //printf("%d\n", n);
        fseek(tagvInfo->fptr_mp3, 2, SEEK_CUR);
        fread(&flag, sizeof(char), 1, tagvInfo->fptr_mp3);
        if(flag == 0)
        {
            fread(tagvInfo->tag_data, sizeof(char), size-1, tagvInfo->fptr_mp3);
            tagvInfo->tag_data[size-1]='\0';
            //printf("%s\n", tagvInfo->tag_data);
        }
        else
        {
            fseek(tagvInfo->fptr_mp3, 1, SEEK_CUR);
            m=(size/2);
            for(uint j=0;j<m;j++)
            {
                fseek(tagvInfo->fptr_mp3, 1, SEEK_CUR);
                fread(&ch, sizeof(char), 1, tagvInfo->fptr_mp3);
                tagvInfo->tag_data[j] = ch;
            }
            tagvInfo->tag_data[m-1]='\0';
            //printf("%s\n", tagvInfo->tag_data);
            fseek(tagvInfo->fptr_mp3, -1, SEEK_CUR);
        }
    }
    return e_success;
}


Status tag_data_storage(TagViewInfo *tagvInfo)
{
    if(strcmp(tagvInfo->tag, "TIT2") == 0)
    {
        strcpy(tagvInfo->title, tagvInfo->tag_data);
    }
    else if(strcmp(tagvInfo->tag, "TPE1") == 0)
    {
        strcpy(tagvInfo->artist, tagvInfo->tag_data);
    }
    else if(strcmp(tagvInfo->tag, "TALB") == 0)
    {
        strcpy(tagvInfo->album, tagvInfo->tag_data);
    }
    else if(strcmp(tagvInfo->tag, "TYER") == 0)
    {
        strcpy(tagvInfo->year, tagvInfo->tag_data);
    }
    else if(strcmp(tagvInfo->tag, "TCON") == 0)
    {
        strcpy(tagvInfo->genre, tagvInfo->tag_data);
    }
    else if(strcmp(tagvInfo->tag, "TCOM") == 0)
    {
        strcpy(tagvInfo->composer, tagvInfo->tag_data);
    }
    return e_success;
}

Status tag_data_view(TagViewInfo *tagvInfo)
{
    dash(63); printf("\n");
    printf("              MP3 TAG READER AND EDITOR FOR ID3v2 \n");
    dash(63); printf("\n");
    printf("%-10s  :       %s\n","TITLE", tagvInfo->title);
    printf("%-10s  :       %s\n","ARTIST", tagvInfo->artist);
    printf("%-10s  :       %s\n","ALBUM", tagvInfo->album);
    printf("%-10s  :       %s\n","YEAR", tagvInfo->year);
    printf("%-10s  :       %s\n","MUSIC", tagvInfo->genre);
    printf("%-10s  :       %s\n","COMPOSER", tagvInfo->composer);
    dash(63); printf("\n\n");
    return e_success;
}
