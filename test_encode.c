#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"
int main(int argc, char *argv[])
{
    if(argc == 1 || argc == 2)
    {
	printf("Invalid input \nPlease pass ....... \nEncode :./a.out -e beautiful.bmp secret.txt stego.bmp \nDecode :./a.out -d stego.bmp decode.txt\n");
	return 1;
    }
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    /*Checking for operation*/
    if(check_operation_type(argv) == e_encode)
    {
	if(argc > 3)
	{
	    printf("Choosen encoding process\n");
	    if(read_and_validate_encode_args(argv,&encInfo) == e_success)
	    {
		printf("SUCCESS : Read and Validation of encode args\n");
		
		printf("Encoding process started...................\n");
		if(do_encoding(&encInfo) == e_success)
		{
		    printf("SUCCESS : Encoding process \n");
		}
		else
		{
		    printf("FAILURE : Encoding process \n");
		    return 1;
		}
	    }
	    else
	    {
		printf("FAILURE :Read and Validation of encode args\n");
		return 1;
	    }
	}
	else 
	{
	    printf("Invalid input \nPlease pass ....... \nEncode :./a.out -e beautiful.bmp secret.txt stego.bmp \nDecode :./a.out -d stego.bmp decode.txt\n");
	    return 1;
	}
    }
    else if(check_operation_type(argv) == e_decode)
    {
	printf("Choosen decoding process\n");
	if (read_and_validate_decode_args(argv, &decInfo) == e_success)
	{
	    printf("SUCCESS : Read and Validation of decode args\n");
	}
	else
	{
	    printf("FAILURE : Read and Validation of decode args\n");
	}
	printf("Decoding process started.......\n");
	
	//Step 2 Open Files
	if (open_files_decode(&decInfo) == e_success)
	{
	    printf("SUCCESS : Open decode files\n");
	}
	else
	{
	    printf("FAILURE : Open decode files\n");
	}
	if (decode_magic_string(MAGIC_STRING, &decInfo) == e_success)
	{
	    printf("SUCCESS : Decoding of magic string\n");
	}
	else
	{
	    printf("FAILURE : Decoding of magic strings\n");
	}

	if (decode_file_ext_size(&decInfo) == e_success)
	{
	    printf("SUCCESS : Decoding of file extension size\n");
	}
	else
	{
	    printf("FAILURE : Decoding of file extension size\n");
	}

	if (decode_secret_file_extn(&decInfo) == e_success)
	{
	    printf("SUCCESS : Decoding of file extension\n");
	}
	else
	{
	    printf("FAILURE : Decoding of file extension\n");
	}
	if (argv[3] == NULL)
	{
	    printf("INFO: Output File not mentioned, Creating decode.txt as default\n");
	}
	decInfo.fptr_secret = fopen(decInfo.secret_fname, "w");
	if (decInfo.fptr_secret == NULL)
	{
	    perror("fopen");
	    fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo.secret_fname);
	    return 1;
	}
	
	printf("SUCCESS : Opened  %s\n", decInfo.secret_fname);
	if (decode_file_size(&decInfo) == e_success)
	{
	    printf("SUCCESS : Decode file size\n" );
	}
	else
	{
	    printf("FAILURE : Decode file size\n");
	}
	if (decode_file_data(&decInfo) == e_success)
	{
	    printf("SUCCESS : Decoding file data\n");
	}
	else
	{
	    printf("FAILURE : Decoding file data\n");
	}
	printf("SUCCESS : Decoding process\n");
	}
    else 
    {
	printf("Invalid input \nPlease pass ....... \nEncode :./a.out -e beautiful.bmp secret.txt stego.bmp \nDecode :./a.out -d stego.bmp decode.txt\n");
	return 1;
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if((strcmp(argv[1],"-e") == 0))
    {
	return e_encode;
    }
    else if((strcmp(argv[1],"-d") == 0))
    {
	return e_decode;
    }
    else
    {
	return e_unsupported;
    }
}
