#include "text_utils.h"
#include "raylib.h"

//Calculate the X center of the screen
int GetTextCenteredX(const char *text, int fontSize, int containerWidth)
{
    int textWidth = MeasureText(text, fontSize);
    return (containerWidth - textWidth) / 2;
}

//Calculate the Y center of the screen
int GetTextCenteredY(const char *text, int fontSize, int containerHeight)
{
    return (containerHeight - fontSize) / 2;
}
