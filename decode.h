#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char secret_fname[20];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file_extn;
    long size_secret_file;

}DecodeInfo;

/*Read and validate*/ 
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open files for decode*/
Status open_files_decode(DecodeInfo *decInfo);

/*do_decoding function*/
Status do_decoding(DecodeInfo *decInfo);

/* Decode Magic string*/
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/*Decode file extension size*/
Status decode_file_ext_size(DecodeInfo *decInfo);

/*Decode serect file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*Decode file size*/
Status decode_file_size(DecodeInfo *decInfo);

/*Decode file data*/
Status decode_file_data(DecodeInfo *decInfo);

/*Decode data from image*/
Status decode_data_from_image(char *data, int size, FILE *fptr_src_image);

/*Decode byte from lsb*/
Status decode_byte_from_lsb(char *data, char *image_buffer);

#endif
