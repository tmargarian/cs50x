#include "helpers.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{   
    // Average RGB and equalize channels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE rgb_avg = round((1.0 * image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
            image[i][j].rgbtRed = rgb_avg;
            image[i][j].rgbtGreen = rgb_avg;
            image[i][j].rgbtBlue = rgb_avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocating memory for the reflected version
    RGBTRIPLE(*image_ref)[width] = calloc(height, sizeof(RGBTRIPLE) * width);

    // Reflecting the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_ref[i][j] = image[i][width - 1 - j];
        }
    }

    // Writing the reflected image back to the original array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_ref[i][j];
        }
    }
    
    free(image_ref);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocating memory for the blurred version
    RGBTRIPLE(*image_bl)[width] = calloc(height, sizeof(RGBTRIPLE) * width);

    // Creting the blurred image version
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Working out edge cases
            int rows[3] = {i - 1, i, i + 1};
            int cols[3] = {j - 1, j, j + 1};

            // RGB intensity values for averaging
            int total_intensity_rgbtRed = 0;
            int total_intensity_rgbtGreen = 0;
            int total_intensity_rgbtBlue = 0;

            // Avg intensity pixel 
            RGBTRIPLE avg_intensity;
            
            // Number of counted pixels for averaging
            int pixels_to_average = 0;

            for (int m = 0; m < 3; m++)
            {
                if (rows[m] < 0 || rows[m] >= height)
                {
                    continue;
                }
                else
                {
                    for (int n = 0; n < 3; n++)
                    {
                        if (cols[n] < 0 || cols[n] >= width)
                        {
                            continue;
                        }
                        else
                        {
                            total_intensity_rgbtRed += image[rows[m]][cols[n]].rgbtRed;
                            total_intensity_rgbtGreen += image[rows[m]][cols[n]].rgbtGreen;
                            total_intensity_rgbtBlue += image[rows[m]][cols[n]].rgbtBlue;
                            pixels_to_average++;
                        }
                    }
                }
            }

            avg_intensity.rgbtRed = round(1.0 * total_intensity_rgbtRed / pixels_to_average);
            avg_intensity.rgbtGreen = round(1.0 * total_intensity_rgbtGreen / pixels_to_average);
            avg_intensity.rgbtBlue = round(1.0 * total_intensity_rgbtBlue / pixels_to_average);

            image_bl[i][j] = avg_intensity;
        }
    }

    // Rewriting the blurred version into the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_bl[i][j];
        }
    }
    
    free(image_bl);    
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocating memory for the edge version
    RGBTRIPLE(*image_e)[width] = calloc(height, sizeof(RGBTRIPLE) * width);

    // The Gx matrix for vertical edge detection
    int Gx[3][3] = 
    {
        {-1, 0, 1}, 
        {-2, 0, 2}, 
        {-1, 0, 1}
    };

    // The Gy matrix for horizontal edge detection
    int Gy[3][3] = 
    {
        {-1, -2, -1},
        { 0,  0,  0}, 
        { 1,  2,  1}
    };

    // Creting the edge image version
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Working out edge cases
            int rows[3] = {i - 1, i, i + 1};
            int cols[3] = {j - 1, j, j + 1};

            // Initializing the Gx sums for each color
            int Gx_sum_red = 0;
            int Gx_sum_green = 0;
            int Gx_sum_blue = 0;
            
            // Initializing Gy sums for each color
            int Gy_sum_red = 0;
            int Gy_sum_green = 0;
            int Gy_sum_blue = 0;

            for (int m = 0; m < 3; m++)
            {
                for (int n = 0; n < 3; n++)
                {
                    if (rows[m] < 0 || rows[m] >= height || cols[n] < 0 || cols[n] >= width)
                    {
                        continue;
                    }
                    else
                    {
                        Gx_sum_red += Gx[m][n] * image[rows[m]][cols[n]].rgbtRed;
                        Gx_sum_green += Gx[m][n] * image[rows[m]][cols[n]].rgbtGreen;
                        Gx_sum_blue += Gx[m][n] * image[rows[m]][cols[n]].rgbtBlue;

                        Gy_sum_red += Gy[m][n] * image[rows[m]][cols[n]].rgbtRed;
                        Gy_sum_green += Gy[m][n] * image[rows[m]][cols[n]].rgbtGreen;
                        Gy_sum_blue += Gy[m][n] * image[rows[m]][cols[n]].rgbtBlue;
                    }
                }
            }

            // Red Value
            if (round(sqrt(pow(Gx_sum_red, 2) + pow(Gy_sum_red, 2))) > 255)
            {
                image_e[i][j].rgbtRed = 255;
            }
            else 
            {
                image_e[i][j].rgbtRed = round(sqrt(pow(Gx_sum_red, 2) + pow(Gy_sum_red, 2)));
            }

            // Green Value
            if (round(sqrt(pow(Gx_sum_green, 2) + pow(Gy_sum_green, 2))) > 255)
            {
                image_e[i][j].rgbtGreen = 255;
            }
            else 
            {
                image_e[i][j].rgbtGreen = round(sqrt(pow(Gx_sum_green, 2) + pow(Gy_sum_green, 2)));
            }

            // Blue Value
            if (round(sqrt(pow(Gx_sum_blue, 2) + pow(Gy_sum_blue, 2))) > 255)
            {
                image_e[i][j].rgbtBlue = 255;
            }
            else 
            {
                image_e[i][j].rgbtBlue = round(sqrt(pow(Gx_sum_blue, 2) + pow(Gy_sum_blue, 2)));
            }
        }
    }

    // Rewriting the edge version into the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_e[i][j];
        }
    }
    
    free(image_e);   
    return;
}
