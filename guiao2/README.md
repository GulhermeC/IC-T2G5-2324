# Compilation

To compile the program, simply run:

``` make ```

# Usage

To run the program, follow this format:

``` ./extract <path_to_image> <color_channel> ``` 

<path_to_image>: Replace with the path to the image you want to process.

<color_channel>: Specify which color channel you want to extract and display. Options are R for Red, G for Green, B for Blue, and ALL to display all channels.

# Examples:

Red Channel:

``` ./extract ./images/airplane.ppm R ```

All Channels:

``` ./extract ./images/airplane.ppm ALL ```