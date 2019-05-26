#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

void process_file(char* filename, vector<int> &numbers)
{
    char inp_buffer[1024];
    // Buffer to store current number
    char current_number[11];
    int current_number_index = 0;
    char current_symbol;
    bool is_number = false;
    bool is_overflow = false;
    int number = 0;
    int bytes_read = 0;
    
    FILE* file = fopen(filename, "r");

    if (file == NULL)
    {
        return;
    }

    while ((bytes_read = fread(inp_buffer, 1, sizeof(inp_buffer), file)) > 0)
    {
        for (int i = 0; i < bytes_read; i++)
        {
            current_symbol = inp_buffer[i];

            if (current_number_index == sizeof(current_number) - 1) 
            {
                current_number[current_number_index] = '\0';
                number = atoi(current_number);
                numbers.push_back(number);
                current_number_index = 0;
                is_number = false;
                is_overflow = true;
                continue;
            }
            if (current_symbol >= '0' && current_symbol <= '9')
            {
                if (is_overflow)
                    continue;
                if (i > 0 && inp_buffer[i-1] == '-')
                {
                    current_number[current_number_index] = '-';
                    current_number_index++;
                }
                is_number = true;
                current_number[current_number_index] = current_symbol;
                current_number_index++;

                continue;
            }
            if (is_number) 
            {
                current_number[current_number_index] = '\0';
                number = atoi(current_number);
                numbers.push_back(number);
            }
            current_number_index = 0;
            is_number = false;
            is_overflow = false;
        }
    }
    if (is_number)
    {
        current_number[current_number_index] = '\0';
        number = atoi(current_number);
        numbers.push_back(number);
    }

    fclose(file);
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Too few arguments\nUsage: program <in_file1> ... <in_fileN> <out_file>\n");

        return 1;
    }

    FILE* out_file = fopen(argv[argc - 1], "w");

    if (out_file == NULL)
    {
        printf("Error while opening output file");

        return 2;
    }

    vector<int> numbers = vector<int>();

    for (int i = 1; i < argc - 1; i++)
        process_file(argv[i], numbers);

    sort(numbers.begin(), numbers.end());

    for(int i = 0; i < numbers.size(); i++ )
        fprintf(out_file, "%i\n", numbers[i]);

    fclose(out_file);
    
    return 0;
}