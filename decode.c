#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <stdlib.h>

/* Function Definitions */

/* 
 * Get operation type
 * Inputs: String pointer
 * Output: Status of operation
 * Return Value: e_enocde or e_decode or e_unsupported
 */

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    char *ptr;

    ptr = strchr(argv[2], '.');
    if(ptr == NULL)
    {
	printf("ERROR : Encoded file format not a bmp\n");
	exit(0);
    }
    if(strcmp(ptr,".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if(argv[3] != NULL)
    {
        strcpy(decInfo->secret_fname, argv[3]);
    }
    else
    {
        strcpy(decInfo->secret_fname, "decode.txt");
    }
    return e_success;
}

/* 
 * Open file decode
 * Inputs: decInfo
 * Output: Open files
 * Return Value: e_success or e_failure
 */
Status open_files_decode(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    //Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

/* 
 * Decode Magic String
 * Inputs: Magic String, DecodeInfo
 * Output: Check whether magic string is  there or note
 * Return Value: e_success or e_failure
 */

Status decode_magic_string( char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);

    int magic_size = strlen(magic_string);
    char data[magic_size];

    decode_data_from_image(data, strlen(magic_string), decInfo -> fptr_stego_image);

    if (strcmp(data,magic_string)){
        return e_failure;
    }
    else
    {
        return e_success;
    }
}

/*decode secret file extension size and return e_success or e_failure */
Status decode_file_ext_size(DecodeInfo *decInfo)
{
    decode_data_from_image((char *)&decInfo->size_secret_file_extn, 4,decInfo -> fptr_stego_image);
    return e_success;
}

/*decode secret file extension and return e_success or e_failure*/
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->extn_secret_file, decInfo->size_secret_file_extn, decInfo -> fptr_stego_image);

    return e_success;
}

/*decode file_size and return e_succes or e_failure*/
Status decode_file_size(DecodeInfo *decInfo)
{
    decode_data_from_image((char *)&decInfo->size_secret_file, 4,decInfo -> fptr_stego_image);
    return e_success;
}

/*Decode  file data and return status*/
Status decode_file_data(DecodeInfo *decInfo)
{
    int i ;
    for (i = 0; i < decInfo->size_secret_file; i++)
    {
        decode_data_from_image(decInfo->secret_data,1,decInfo -> fptr_stego_image);

        if(!fwrite(decInfo->secret_data,1,1,decInfo->fptr_secret))
            return e_failure;
    }
    return e_success;
}

/*Decode data from image*/

Status decode_data_from_image(char *data, int size, FILE *fptr_src_image)
{
    int i ;
    char img_buffer[MAX_IMAGE_BUF_SIZE];

    for (i = 0; i < size; i++)
    {
        if (!fread(img_buffer, sizeof(img_buffer), 1, fptr_src_image))
            return e_failure;

        if (decode_byte_from_lsb(&data[i],img_buffer) == e_failure)
        {
            fprintf(stderr, "ERROR: %s function failed\n", "encode_byte_tolsb" );
            return e_failure;
        }
    }

    return e_success;
}

/*decode byte from lbs*/
Status decode_byte_from_lsb(char * data, char *image_buffer)
{
    *data =0;
    int i;
    for (i = MAX_IMAGE_BUF_SIZE -1 ; i >= 0; i--)
    {
        *data |= (image_buffer[MAX_IMAGE_BUF_SIZE - 1 - i] & 1) << i;
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_files_decode(decInfo) == e_success)
    {
        printf("SUCCESS : Open file\n");
        if (decode_file_ext_size(decInfo) == e_success)
        {
            printf("SUCCESS : Decoding file extension size\n");
            if (decode_secret_file_extn(decInfo) == e_success)
            {
                printf("SUCCESS : Decoding file extension\n");
                if (decode_file_size(decInfo) == e_success)
                {
                    printf("SUCCESS : Decoding file size\n" );
                    if (decode_file_data(decInfo) == e_success)
                    {
                        printf("SUCCESS : Decoding file data\n");
                    }
                    else
                    {
                        printf("FAILURE : Decoding file data\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("FAILURE : Decoding file size\n");
                    return e_failure;
                }
            }
            else
            {
                printf("FAILURE : Decoding file extension\n");
                return e_failure;
            }
        }
        else
        {
            printf("FAILURE : Decoding file extension size\n");
            return e_failure;
        }
    }
    else
    {
        printf("FAILURE : Open file\n");
        return e_failure;
    }
    return e_success;
}




