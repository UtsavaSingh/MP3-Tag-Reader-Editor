#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_error,
    e_help,
    e_view,
    e_edit,
    e_unsupported
} OperationType;

typedef enum
{
    e_edit_title,
    e_edit_album,
    e_edit_year,
    e_edit_artist,
    e_edit_genre,
    e_edit_composer
} EditOperationType;

#endif
