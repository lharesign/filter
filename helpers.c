#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) //Iterating through 2D array of height, width
        {
            //Calculating average colour and covneerting RGB values to said average
            float average_color = round((image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.000);
            image[i][j].rgbtRed = average_color;
            image[i][j].rgbtBlue = average_color;
            image[i][j].rgbtGreen = average_color;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        //Create temp array equal to width
        int j = width;
        RGBTRIPLE temp_array[j];

        for (int l = 0; l < width; l++) //Iterating through 2D array of height, width
        {
            //Set right side of temp array to equal left side of original array, moving through each pixel
            temp_array[j - 1] = image[i][l];
            j--;
        }
        //Set original array to equal temp_array
        for (int m = 0; m < width; m++)
        {
            image[i][m] = temp_array[m];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Create temp array equal to width
    int j = height;
    int k = width;
    RGBTRIPLE sub_array[j][k];

    for (int i = 0; i < height; i++)
    {
        for (int l = 0; l < width; l++) //Iterating through 2D array of height, width
        {
            float total_red = 0.0;
            float total_blue = 0.0;
            float total_green = 0.0;
            float cell_counter = 0.0;

            for (int m = i - 1; m < i + 2; m++) //Iterating through all surrounding boxes to calc avg colour
            {
                if (m < 0 || m >= height)
                {
                    continue;
                }
                else
                {
                    for (int n = l - 1; n < l + 2; n++)
                    {
                        if (n < 0 || n >= width)
                        {
                            continue;
                        }
                        else
                        {
                            //Adding values to totals and cell_counter++ to discover average
                            total_red = total_red + image[m][n].rgbtRed;
                            total_blue = total_blue + image[m][n].rgbtBlue;
                            total_green = total_green + image[m][n].rgbtGreen;
                            cell_counter++;
                        }
                    }
                }
            }
            //Setting RGB to blurred values
            sub_array[i][l].rgbtRed = round(total_red / cell_counter);
            sub_array[i][l].rgbtBlue = round(total_blue / cell_counter);
            sub_array[i][l].rgbtGreen = round(total_green / cell_counter);
        }
    }

    for (int o = 0; o < height; o++)
    {
        for (int p = 0; p < width; p++)
        {
            image[o][p] = sub_array[o][p];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Create temp array equal to width
    int j = height;
    int k = width;
    RGBTRIPLE sub_array[j][k];

    for (int i = 0; i < height; i++)
    {
        for (int l = 0; l < width; l++) //Iterating through 2D array of height, width
        {
            float gx_red = 0.0;
            float gx_blue = 0.0;
            float gx_green = 0.0;
            float gx_value = -1.0;
            float gy_red = 0.0;
            float gy_blue = 0.0;
            float gy_green = 0.0;
            float gy_value = -1.0;

            for (int m = i - 1; m < i + 2; m++) //Iterating through all surrounding boxes to calc Gx values
            {
                if (m == i - 1 || m == i + 1)
                {
                    gx_value = -1.00; //Resetting gx_value after iterating through each row
                }
                else
                {
                    gx_value = -2.0;
                }

                if (m < 0 || m >= height) //Adding zero values for pixels outside the height range
                {
                    continue;
                }

                else
                {
                    for (int n = l - 1; n < l + 2; n++)
                    {
                        if (n < 0 || n >= width) //Adding zero values for pixels outside the width range
                        {
                            if (m == i)
                            {
                                gx_value += 2; //Adding an extra 1 to Gx Value when calculating middle row
                            }
                            else
                            {
                                gx_value++;
                            }
                            continue;
                        }
                        else
                        {
                            //Adding values to Gx values
                            gx_red = gx_red + (image[m][n].rgbtRed * gx_value);
                            gx_blue = gx_blue + (image[m][n].rgbtBlue * gx_value);
                            gx_green = gx_green + (image[m][n].rgbtGreen * gx_value);

                            if (m == i)
                            {
                                gx_value += 2; //Adding an extra 1 to Gx Value when calculating middle row
                            }
                            else
                            {
                                gx_value++;
                            }

                            if ((m == i - 1 && n == l - 1) || (m == i - 1 && n == l + 1)) //Rules for gy value
                            {
                                gy_value = -1.0;
                            }
                            else if (m == i - 1 && n == l)
                            {
                                gy_value = -2.0;
                            }
                            else if ((m == i + 1 && n == l - 1) || (m == i + 1 && n == l + 1))
                            {
                                gy_value = 1.0;
                            }
                            else if (m == i + 1 && n == l)
                            {
                                gy_value = 2.0;
                            }
                            else
                            {
                                gy_value = 0.0;
                            }

                            gy_red = gy_red + (image[m][n].rgbtRed * gy_value);
                            gy_blue = gy_blue + (image[m][n].rgbtBlue * gy_value);
                            gy_green = gy_green + (image[m][n].rgbtGreen * gy_value);
                        }
                    }
                }
            }

            //Setting RGB to Sobel values
            float final_red = round(sqrt((gx_red * gx_red) + (gy_red * gy_red)));
            float final_blue = round(sqrt((gx_blue * gx_blue) + (gy_blue * gy_blue)));
            float final_green = round(sqrt((gx_green * gx_green) + (gy_green * gy_green)));

            if (final_red > 255)
            {
                final_red = 255;
            }
            if (final_blue > 255)
            {
                final_blue = 255;
            }
            if (final_green > 255)
            {
                final_green = 255;
            }

            sub_array[i][l].rgbtRed = final_red;
            sub_array[i][l].rgbtBlue = final_blue;
            sub_array[i][l].rgbtGreen = final_green;
        }
    }

    for (int o = 0; o < height; o++)
    {
        for (int p = 0; p < width; p++)
        {
            image[o][p] = sub_array[o][p];
        }
    }
    return;
}