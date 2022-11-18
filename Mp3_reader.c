/*
 * Name            : Utsava Kumar Singh
 * Date            : 17-11-2022
 * Description     : MP3 Reader and Editor
 * Input           :

 * Output          :
*/

#include <stdio.h>
#include <string.h>
#include "mp3_view.h"
#include "mp3_edit.h"
#include "types.h"

/* Check operation type */
OperationType check_operation_type(int argc, char *argv[]);
void dash(int n);

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
            printf("\t 2.5 -t -> to edit content\n");
            printf("\t 2.6 -c -> to edit comment\n\n");
            dash(55); printf("\n");
        }
        else if(operation == e_view)
        {
            printf(" SELECTED VIEW DETAILS \n");
            TagViewInfo tagvInfo;
            if(read_and_validate_view_args(argv, &tagvInfo) == e_success)
            {
                printf("INFO : Read and validate function is successfully executed\n");
                // viewing tags
                if(do_viewing(&tagvInfo) == e_success)
                {
                    printf("INFO : ## Viewing done successfully ##\n");
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
            printf("USAGE :\n");
        }
        else
        {
            printf("USAGE :\n");
        }
    }
    else
    {
        printf("USAGE :\n");
    }

    return 0;
}

/* Check for operation to be performed
 * Input: Command line arguments
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

void dash(int n)
{
    for(int i = 0; i < n; i++)
    	printf("-");
}
