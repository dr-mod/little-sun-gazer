#include "EPD_3in7.h"
#include "EPD_3in7.c"
#include "GUI_Paint.c"

class Drawer {
private:
    UBYTE *BlackImage;
    short stored_color;
    DOT_PIXEL pen_size;
    LINE_STYLE line_style;
public:

    Drawer() {
        pen_size = DOT_PIXEL_1X1;
        line_style = LINE_STYLE_SOLID;

        if (DEV_Module_Init() != 0) {
        }
        EPD_3IN7_4Gray_Init();
        EPD_3IN7_4Gray_Clear();
        DEV_Delay_ms(1000);
        EPD_3IN7_1Gray_Init();
        EPD_3IN7_1Gray_Clear();

        UWORD Imagesize =
                ((EPD_3IN7_WIDTH % 4 == 0) ? (EPD_3IN7_WIDTH / 4) : (EPD_3IN7_WIDTH / 4 + 1)) * EPD_3IN7_HEIGHT;
        if ((BlackImage = (UBYTE *) malloc(Imagesize)) == NULL) {
            printf("Failed to apply for black memory...\r\n");
        }

        Paint_NewImage(BlackImage, EPD_3IN7_WIDTH, EPD_3IN7_HEIGHT, 0, WHITE);
        Paint_SetScale(4);
        Paint_Clear(WHITE);

        printf("SelectImage:BlackImage\r\n");
        Paint_SelectImage(BlackImage);
        Paint_SetScale(2);
        Paint_Clear(WHITE);
    }

    ~Drawer() {
        EPD_3IN7_4Gray_Init();
        printf("Clear...\r\n");
        EPD_3IN7_4Gray_Clear();

        // Sleep & close 5V
        printf("Goto Sleep...\r\n");
        EPD_3IN7_Sleep();

        free(BlackImage);
        BlackImage = NULL;
        DEV_Delay_ms(2000);//important, at least 2s
        printf("close 5V, Module enters 0 power consumption ...\r\n");
        DEV_Module_Exit();
    }

    void pen_width(short width) {
        switch (width) {
            case 1:
                pen_size = DOT_PIXEL_1X1;
                break;
            case 2:
                pen_size = DOT_PIXEL_2X2;
                break;
            case 3:
                pen_size = DOT_PIXEL_3X3;
                break;
            default:
                pen_size = DOT_PIXEL_1X1;
        }
    }

    void pen_style(short style) {
        switch (style) {
            case 0:
                line_style = LINE_STYLE_DOTTED;
                break;
            default:
                line_style = LINE_STYLE_SOLID;
        }
    }

    void pen_color(short color) {
        switch (color) {
            case 0:
                stored_color = BLACK;
                break;
            case 1:
                stored_color = BLACK;
                break;
            case 2:
                stored_color = BLACK;
                break;
            case 3:
                stored_color = BLACK;
                break;
            default:
                stored_color = WHITE;
        }
    }

    void line(short x1, short y1, short x2, short y2) {
        Paint_DrawLine(x1, y1, x2, y2, stored_color, pen_size, line_style);
    }

    void circle(short x, short y, short r) {
        Paint_DrawCircle(x, y, r, stored_color, pen_size, DRAW_FILL_FULL);
    }

    void circle_empty(short x, short y, short r) {
        Paint_DrawCircle(x, y, r, stored_color, pen_size, DRAW_FILL_EMPTY);
    }

    void commit() {
        EPD_3IN7_1Gray_Display_Part(BlackImage, 0, 0, EPD_3IN7_WIDTH, EPD_3IN7_HEIGHT);
    }

};
