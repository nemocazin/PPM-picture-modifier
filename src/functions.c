/**
 * @brief     C file for the functions file
 *            These functions are used to replaced the math.h lib 
 * 
 * @author    CAZIN Némo
 * @date      2024
 */

#include "functions.h"



/**
 * @brief Load a picture from a repertory 
 * 
 * @param image The image to load
 * 
 * @return 1 if loaded, 0 otherwise
 */
int Load (unsigned char image[HEIGHT][WIDTH][PIX_SIZE])
{
    char way [WAY_SIZE];
    printf("Put the way of the your original picture :\n");
    printf("Example : /home/c/m/cmartin/images/polarbear.ppm\n");
    scanf("%s", way);
    if (LoadImage(way, image))
    {
        printf("The picture has been loaded.\n");
        return LOADED;
    }
    printf("Couldn't load the picture.\n");
    return NOTLOADED;
}



/**
 * @brief Save a picture into a repertory 
 * 
 * @param image The image to save
 * 
 * @return 1 if saved, 0 otherwise
 */
int Save (unsigned char image[HEIGHT][WIDTH][PIX_SIZE])
{
    char new_way[WAY_SIZE];
    printf("Put the way to your new picture :\n");
    printf("Example : /home/c/m/cmartin/images/polarbear_modified.ppm\n");
    scanf("%s", new_way);
    if (SaveImage(new_way, image))
    {
        printf("The picture has been saved.\n");
        return SAVED;
    }
    printf("Couldn't save the picture.\n");
    return NOTLOADED;
}



/**
 * @brief Copy a picture from a repertory 
 * 
 * @param image The image to copy
 * 
 * @return 1 if copy done, 0 otherwise
 */
int Copy (unsigned char image[HEIGHT][WIDTH][PIX_SIZE])
{
    printf("You have selected the copy function.\n");
    if (Load(image))
    {
        Save(image);
        return SAVED;
    }
    return NOTLOADED;
}



/**
 * @brief Blur a picture from a repertory 
 * 
 * @param image The image to 
 * 
 * @return 1 if blur done, 0 otherwise
 */
int Blur (unsigned char image[HEIGHT][WIDTH][PIX_SIZE])
{
    printf("You have selected the blur function.\n");
    unsigned char blur_image[HEIGHT][WIDTH][PIX_SIZE];
    int size_blur = 0;
    int save_color_value = 0;
    int average_color_value = 0;
    if (Load(image)){
        printf("Put the radius of the blur : \n");
        scanf("%d", &size_blur);
        save_color_value = size_blur;
        // Addition of the 8 pixels value around one pixel to this pixel
        // For each :
        for(int image_x = 0; image_x < HEIGHT; image_x++)
        {
            for (int image_y = 0; image_y < WIDTH; image_y++)
            {
                for (int image_color = 0; image_color < PIX_SIZE; image_color++)
                {
                    for (size_blur = 0; size_blur < save_color_value; size_blur++)
                    {
                        if(image_x - size_blur > 0 && image_x + size_blur < HEIGHT && image_y + size_blur < WIDTH && image_y - size_blur > 0)
                        {
                            average_color_value = (
                                                  image[image_x][image_y-size_blur][image_color]
                                                + image[image_x+size_blur][image_y][image_color]
                                                + image[image_x][image_y+size_blur][image_color]
                                                + image[image_x-size_blur][image_y][image_color]
                                                + image[image_x-size_blur][image_y-size_blur][image_color]
                                                + image[image_x-size_blur][image_y+size_blur][image_color]
                                                + image[image_x+size_blur][image_y-size_blur][image_color]
                                                + image[image_x+size_blur][image_y+size_blur][image_color]
                                                )/8;
                        }
                        else average_color_value+=0;
                    }
                    blur_image[image_x][image_y][image_color] = average_color_value;
                }
            }
        }
        Save(blur_image);
        return SAVED;
    } 
    return NOTLOADED;	  
}



/**
 * @brief Change the color to monochrom of a picture
 * 
 * @param image The image to make monochrom
 * 
 * @return 1 if monochrom done, 0 otherwise
 */
int Monochrom(unsigned char image[HEIGHT][WIDTH][PIX_SIZE])
{
    printf("You have selected the monochrom function\n");
    unsigned int color = 1000;
    float hue = 0, saturation = 0, value = 0;
    if (Load(image)){
        while (color > 360)
        {
            printf("Write the color of the monochrom (0 to 360)\n");
            scanf("%d", &color);
            printf("color : %d\n", color);
        }

        for(int image_x = 0; image_x < HEIGHT; image_x++)
        {
            for(int image_y = 0; image_y < WIDTH; image_y++)
            {
                RGB_To_HSV(&image[image_x][image_y][0], &image[image_x][image_y][1], &image[image_x][image_y][2]);
                hue = color;
                saturation = image[image_x][image_y][1];
                value = image[image_x][image_y][2];
                HSV_To_RGB(&hue, &saturation, &value);
                image[image_x][image_y][0] = hue;
                image[image_x][image_y][1] = saturation;
                image[image_x][image_y][2] = value;
            }
        }
        Save(image);
        return SAVED;
    }
    return NOTLOADED;
}
  


/**
 * @brief Make a mosaic from a picture
 * 
 * @param image The image to make use to make the mosaic
 */
int Mosaic(unsigned char image[HEIGHT][WIDTH][PIX_SIZE])
{
    printf("You have selected the mosaic function\n");
    unsigned char mosaic[HEIGHT/2][WIDTH/2][PIX_SIZE];
    int mos_x = 0, mos_y = 0;
    int x_image = 0, y_image = 0, pixel_select = 0;
    int center[2] = {HEIGHT/2, WIDTH/2};
    if (Load(image) == 1)
    {
        // CREATION OF THE SMALL PICTURE
        for (x_image = 0; x_image<HEIGHT; x_image=x_image+2)
        {
            for(y_image = 0; y_image<WIDTH; y_image=y_image+2)
            {
                for(pixel_select = 0; pixel_select<PIX_SIZE; pixel_select++)
                {
                    mosaic[mos_x][mos_y][pixel_select] = image[x_image][y_image][pixel_select];
                }
                mos_y ++;
            }
            mos_x ++;
            mos_y=0;
        }

        // TOP LEFT SQUARE
        for (x_image = 0; x_image<center[0]; x_image++)
        {
            for(y_image = 0; y_image<center[1]; y_image++)
            {
                for(pixel_select = 0; pixel_select<PIX_SIZE; pixel_select++){
                    image[x_image][y_image][pixel_select] = mosaic[x_image][y_image][pixel_select];
                }       
            }
        }

        // TOP RIGHT SQUARE
        for (x_image = 0; x_image<center[0]; x_image++)
        {
            for(y_image = 0; y_image<center[1]; y_image++)
            {
                for(pixel_select = 0; pixel_select<PIX_SIZE; pixel_select++)
                {
                    image[x_image+center[0]][y_image][pixel_select] = mosaic[x_image][y_image][pixel_select];
                }
            }
        }

        // BOTTOM LEFT SQUARE
        for (x_image = 0; x_image<center[0]; x_image++)
        {
            for(y_image = 0; y_image<center[1]; y_image++)
            {
                for(pixel_select = 0; pixel_select<PIX_SIZE; pixel_select++)
                {
                    image[x_image][y_image+center[1]][pixel_select] = mosaic[x_image][y_image][pixel_select];
                }
            }
        }

        // BOTTOM RIGHT SQUARE
        for (x_image = 0; x_image<center[0]; x_image++)
        {
            for(y_image = 0; y_image<center[1]; y_image++)
            {
                for(pixel_select = 0; pixel_select<PIX_SIZE; pixel_select++){
                    image[x_image+center[0]][y_image+center[1]][pixel_select] = mosaic[x_image][y_image][pixel_select];
                }
            }
        }

        Save(image);
        return SAVED;
    }
    return NOTLOADED;
}