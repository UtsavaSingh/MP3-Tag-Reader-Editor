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
    if(open_mp3_file(tagvInfo) == e_success)
    {
        printf("INFO : Done\n");
    }
    else
    {
        printf("ERROR : Opening mp3 file is failure\n");
        return e_failure;
    }
    char str[4];
    uint n=0;
    fread(&str, sizeof(char), 4, tagvInfo->fptr_mp3);
    printf("%s\n", str);
    fseek(tagvInfo->fptr_mp3, 10, SEEK_SET);
    for(int i=0; i<5;i++)
    {
        fread(&str, sizeof(char), 4, tagvInfo->fptr_mp3);
        printf("%s\n", str);
        fseek(tagvInfo->fptr_mp3, 3, SEEK_CUR);
        fread(&n, sizeof(char), 1, tagvInfo->fptr_mp3);
        printf("%d\n", n);
        fseek(tagvInfo->fptr_mp3, 3, SEEK_CUR);
        fread(tagvInfo->title, sizeof(char), n-1, tagvInfo->fptr_mp3);
        printf("%s\n", tagvInfo->title);
    }
    while(fgetc(tagvInfo->fptr_mp3) == '0');
    char ch=fgetc(tagvInfo->fptr_mp3);
    printf("%c", ch);

    return e_success;
}

/*
 * Get File pointer for mp3 file
 * Inputs: File info
 * Output: FILE pointer for mp3 file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_mp3_file(TagViewInfo *tagvInfo)
{
    printf("INFO : Opening mp3 file\n");
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
        printf("INFO : Opened %s\n", tagvInfo -> mp3_fname);
        return e_success;
    }
}
