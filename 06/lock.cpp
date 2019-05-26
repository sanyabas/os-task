#include <string.h>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

string get_full_filename(string filename)
{
    return filename + ".lck";
}

bool is_acquired(string lockfile)
{
    FILE *file = fopen(lockfile.c_str(), "r");
    bool is_acquired = file != NULL;

    if (is_acquired)
        fclose(file);

    return is_acquired;
}

void release(string lockfile)
{
    remove(lockfile.c_str());
}

bool acquire(string lockfile, char *operation)
{
    FILE *file = fopen(lockfile.c_str(), "w");
    if (file == NULL)
        return false;

    if (fprintf(file, "%i:%s", getpid(), operation) < 0)
    {
        release(lockfile);
        return false;
    }

    fclose(file);
    return true;
}

// Read data from stdin and write to file
bool write_cmd(const char *filename)
{
    int bytes_read, bytes_written;
    char buffer[1024];
    FILE *file = fopen(filename, "w");

    if (file == NULL)
        return false;

    while ((bytes_read = read(0, buffer, sizeof(buffer))) > 0)
    {
        bytes_written = fwrite(buffer, 1, bytes_read, file);
        if (bytes_written < 0)
        {
            fclose(file);
            return false;
        }
    }

    fclose(file);
    return true;
}

// Print file contents to stdout
bool read_cmd(const char *filename)
{
    int bytes_read, bytes_written;
    char buffer[1024];
    FILE *file = fopen(filename, "r");

    if (file == NULL)
        return false;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (bytes_read < 0)
        {
            fclose(file);
            return false;
        }
        bytes_written = write(1, buffer, bytes_read);
    }

    fclose(file);
    return true;
}

main(int argc, char *argv[])
{
    if (argc < 3 || (strcmp(argv[1], "read") != 0 && strcmp(argv[1], "write") != 0))
    {
        printf("Usage: ./lock read|write <filename>\n");
        return 1;
    }

    string filename = argv[2];
    string lockfile = get_full_filename(filename);
    bool (*handler)(const char *) = strcmp(argv[1], "read") == 0 ? &read_cmd : &write_cmd;

    while (is_acquired(lockfile))
        sleep(1);

    if (!acquire(lockfile, argv[1]))
    {
        printf("Unable to aquire lock\n");
        return 1;
    }

    bool result = (*handler)(filename.c_str());

    if (!result)
    {
        printf("IO error");
        return 1;
    }
    release(lockfile);

    return result;
}
