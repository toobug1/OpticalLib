#ifndef TRGB_H
#define TRGB_H


struct TRgb
{
    TRgb();
    TRgb(float red, float green, float blue, float alpha = 1.0);
    bool operator< (const TRgb &TRgb) const;
    TRgb operator~ () const;

    float r;
    float g;
    float b;
    float a;
};

extern const TRgb rgb_black;
extern const TRgb rgb_red;
extern const TRgb rgb_green;
extern const TRgb rgb_blue;
extern const TRgb rgb_yellow;
extern const TRgb rgb_cyan;
extern const TRgb rgb_magenta;
extern const TRgb rgb_gray;
extern const TRgb rgb_white;

/** This constant specifies the number of entries in @ref rgb_table. @showvalue */
static const int rgb_table_size = 9;
/** This table contains some common colors. @see rgb_table_size */
extern const TRgb rgb_table[];

#endif // TRGB_H
