#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define FAT_BLOCK 512
#define MAX_PICS 50

int main(int argc, char *argv[])
{
    // Checking that there is only one argument passed with the program
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover <raw file name>");
        return 1;
    }

    // Opening the file and attempting to open it
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    // Defining a 512 B buffer
    uint8_t fat_buffer[FAT_BLOCK]; 

    // JPG file counter
    int files_found = 0;

    // An Array of FILEs (50 Pictures max)
    FILE *jpg_files[MAX_PICS];

    // Reading the raw file until the end
    while (fread(fat_buffer, sizeof(uint8_t), FAT_BLOCK, file) == FAT_BLOCK)
    {
        // Checking for the beginning of the JPEG header
        if (fat_buffer[0] == 0xff && fat_buffer[1] == 0xd8 && 
            fat_buffer[2] == 0xff && (fat_buffer[3] & 0xf0) == 0xe0)
        {
            if (files_found == 0)
            {
                char jpg_name[8];
                sprintf(jpg_name, "%03i.jpg", files_found);
                
                jpg_files[files_found] = fopen(jpg_name, "w");
                if (jpg_files[files_found] == NULL)
                {
                    fclose(jpg_files[files_found]);
                    fprintf(stderr, "Could not create %s.\n", jpg_name);
                    return 1;
                }

                fwrite(fat_buffer, sizeof(uint8_t), FAT_BLOCK, jpg_files[files_found]);
                files_found++;
            }
            else
            {
                fclose(jpg_files[files_found - 1]);

                char jpg_name[8];
                sprintf(jpg_name, "%03i.jpg", files_found);
                
                jpg_files[files_found] = fopen(jpg_name, "w");
                if (jpg_files[files_found] == NULL)
                {
                    fclose(jpg_files[files_found]);
                    fprintf(stderr, "Could not create %s.\n", jpg_name);
                    return 1;
                }

                fwrite(fat_buffer, sizeof(uint8_t), FAT_BLOCK, jpg_files[files_found]);
                files_found++;
            }
        }
        else
        {   
            if (files_found > 0)
            {
                fwrite(fat_buffer, sizeof(uint8_t), FAT_BLOCK, jpg_files[files_found - 1]);
            }
        }
    }
    fclose(jpg_files[files_found - 1]); // closing the last file
}