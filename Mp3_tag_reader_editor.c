/*
 * Name            : Utsava Kumar Singh
 * Date            : 17-11-2022
 * Description     : MP3 Tag Reader and Editor
 * Input           : For option 'v'
                     a) Source mp3 file name (.mp3)
                     b) Secret text file name (.c / .sh / .txt)
                     c) Output image file name (.bmp) (optional)
                       Example - ./Mp3_reader_editor -v <.mp3 file>
                     For option 'e'
                     a) Option to edit particular tag (-t/-a/-A/-m/-y/-c)
                     b) Tag data to replace the chosen tag (" ")
                     b) Source mp3 file name (.mp3)
                       Example - ./Mp3_reader_editor -e <-t/-a/-A/-m/-y/-c> <"changing text"> <.mp3 file>

 * Output          : Show the tag data of the mp3 file
*/

#include <stdio.h>
#include <string.h>
#include "mp3_view.h"
#include "mp3_edit.h"
#include "types.h"

/* Check operation type */
OperationType check_operation_type(int argc, char *argv[]);

int main(int argc, char **argv)
{
    //validate the command line arguments count
    if(argc < 6)
    {
        //Input argument validation
        int operation = check_operation_type(argc, argv);

        if(operation == e_error)
        {
            printf("\n"); dash(80);
            printf("\n\nERROR: %s : INVALID ARGUMENTS\n", argv[0]);
            printf("USAGE :\n");
            printf("To view please pass like : %s -v mp3filename\n", argv[0]);
            printf("To edit please pass like : %s -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n", argv[0]);
            printf("To get help pass like : %s --help\n\n", argv[0]);
            dash(80); printf("\n");
        }
        else if(operation == e_help)
        {
            printf("\n"); dash(23); printf("HELP MENU"); dash(23);
            printf("\n\n1. -v -> to view mp3 file contents\n");
            printf("2. -e -> to edit mp3 file contents\n");
            printf("\t 2.1 -t -> to edit song title\n");
            printf("\t 2.2 -a -> to edit artist name\n");
            printf("\t 2.3 -A -> to edit album name\n");
            printf("\t 2.4 -y -> to edit year\n");
            printf("\t 2.5 -g -> to edit content type\n");
            printf("\t 2.6 -c -> to edit composer\n\n");
            dash(55); printf("\n");
        }
        else if(operation == e_view)
        {
            printf("\n"); dash(20); printf(" SELECTED VIEW DETAILS "); dash(20); printf("\n\n\n");
            TagViewInfo tagvInfo;
            if(read_and_validate_view_args(argv, &tagvInfo) == e_success)
            {
                // viewing tags
                if(do_viewing(&tagvInfo) == e_success)
                {
                    dash(15); printf(" DETAILS DISPLAYED SUCCESSFULLY "); dash(16); printf("\n\n");
                    // closing file
                    fclose(tagvInfo.fptr_mp3);
                }
                else
                {
                    printf("ERROR : Viewing operation failed\n");
                    return 1;
                }
            }
            else
            {
                printf("ERROR : Read and validate function is failure\n");
                return 1;
            }
        }
        else if(operation == e_edit)
        {
            printf("\n"); dash(20); printf(" SELECTED EDIT DETAILS "); dash(20); printf("\n");
            TagEditInfo tageInfo;
            if(read_and_validate_edit_args(argv, &tageInfo) == e_success)
            {
                // Editing tags
                if(do_editing(&tageInfo) == e_success)
                {
                    dash(15); printf(" %s EDITED SUCCESSFULLY ", tageInfo.edited_title_name); dash(16); printf("\n\n");
                    // closing files
                    fclose(tageInfo.fptr_mp3_old);
                    fclose(tageInfo.fptr_mp3_new);
                    // removing old mp3 file
                    remove(tageInfo.mp3_fname);
                    // renaming the new mp3 file by the old mp3 file name
                    rename("new.mp3", tageInfo.mp3_fname);
                }
                else
                {
                    printf("ERROR : Editing operation failed\n");
                    return 1;
                }
            }
            else
            {
                printf("ERROR : Read and validate function is failure\n");
                return 1;
            }
        }
        else
        {
            printf("ERROR : Operation is invalid\n");
            printf("To view please pass like : %s -v mp3filename\n", argv[0]);
            printf("To edit please pass like : %s -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n", argv[0]);
            printf("To get help pass like : %s --help\n\n", argv[0]);
        }
    }
    else
    {
        printf("ERROR : Please execute file like below\n");
        printf("To view please pass like : %s -v mp3filename\n", argv[0]);
        printf("To edit please pass like : %s -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n", argv[0]);
        printf("To get help pass like : %s --help\n\n", argv[0]);
    }
    return 0;
}

/* Check for operation to be performed
 * Input: Command line arguments and count
 * Output: Operation to be performed
 * Return: integer constant corresponding to operation
 */
OperationType check_operation_type(int argc, char *argv[])
{
    if(argc == 1)
    {
    	return e_error;
    }
    else if((argc == 2) && ((strcmp(argv[1], "--help")) == 0))
    {
    	return e_help;
    }
    else if((argc == 3) && ((strcmp(argv[1], "-v")) == 0))
    {
    	return e_view;
    }
    else if((argc == 5) && ((strcmp(argv[1], "-e")) == 0))
    {
    	return e_edit;
    }
    else
    {
        return e_unsupported;
    }
}

/* Function definition to print n number of dash */
void dash(int n)
{
    for(int i = 0; i < n; i++)
    	printf("-");
}
