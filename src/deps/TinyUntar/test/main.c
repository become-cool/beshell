#include <stdio.h>

#include "untar.h"

FILE *fp_writer = NULL;

int entry_header_cb(header_translated_t *proper, 
                    int entry_index, 
                    void *context_data)
{
    dump_header(proper);
    
    if(proper->type == T_NORMAL)
    {
        char file_path[256] = "";
        strcat(file_path, "/tmp/");
        strcat(file_path, proper->filename);

        if((fp_writer = fopen(file_path, "wb")) == NULL)
        {
            printf("Could not open [%s] for write.\n", file_path);
            return -1;
        }
    }
    else
        printf("Not writing non-normal file.\n\n");

    return 0;
}

int entry_data_cb(header_translated_t *proper, 
                  int entry_index, 
                  void *context_data,
                  unsigned char *block, 
                  int length)
{
    if(fp_writer != NULL)
        fwrite(block, length, 1, fp_writer);

    return 0;
}

int entry_end_cb(header_translated_t *proper, 
                 int entry_index, 
                 void *context_data)
{
    if(fp_writer != NULL)
    {
        fclose(fp_writer);
        fp_writer = NULL;
    }

    return 0;
}

int main(int argc, const char* argv[])
{
    entry_callbacks_t entry_callbacks = { entry_header_cb, 
                                          entry_data_cb,
                                          entry_end_cb };

    if(argc < 2)
    {
        printf("Please provide the file-path of a TAR file.\n\n");
        return -1;
    }

    if(read_tar(argv[1], &entry_callbacks, NULL) != 0)
    {
        printf("Read failed.\n\n");
        return -2;
    }

    return 0;
}

