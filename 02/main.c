#include <stdio.h>
#include <unistd.h>

void process_buf(char* in_buf, char* out_buf, int buf_size, FILE* out_file)
{
    int out_pos = 0;
    int zeros_count = 0;
    int is_sparse = 0;

    for (int i = 0; i < buf_size; i++)
    {
        // Accumulating zero bytes
        if (in_buf[i] == 0)
        {
            is_sparse = 1;
            zeros_count++;
            
            continue;
        }
        // Flushing data to file
        if (is_sparse)
        {
            fwrite(out_buf, 1, out_pos, out_file);
            fseek(out_file, zeros_count, SEEK_CUR);
            zeros_count = 0;
            is_sparse = 0;
            out_pos = 0;
        }
        // Accumulating non-zero bytes
        out_buf[out_pos] = in_buf[i];
        out_pos++;
    }
    
    if (is_sparse)
    {
        fseek(out_file, zeros_count, SEEK_CUR);
    }
    
    fwrite(out_buf, 1, out_pos, out_file);
}

void process_file(char* filename)
{
    char in_buf[1024];
    char out_buf[1024];
    
    int bytesRead = 0;
    FILE* out_file = fopen(filename, "wb");

    if (out_file == NULL)
    {
        printf("Error while opening output file\n");

        _exit(2);
    }

    while ((bytesRead = read(0, in_buf, sizeof(in_buf))) > 0)
        process_buf(in_buf, out_buf, bytesRead, out_file);   

    fclose(out_file);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Please specify output file name\n");

        return 1;
    }

    process_file(argv[1]);

    return 0;
}