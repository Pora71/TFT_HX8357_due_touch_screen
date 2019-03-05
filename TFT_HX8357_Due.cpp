/*

      Add this part of code in the end of the file: TFT_HX8357_Due.cpp (https://github.com/Bodmer/TFT_HX8357_Due/blob/master/TFT_HX8357_Due.cpp)

*/




TFT_HX8357_Due_Button::TFT_HX8357_Due_Button(void) {
    _gfx = 0;
}

// Classic initButton() function: pass center & size
void TFT_HX8357_Due_Button::initButton(
                                     TFT_HX8357_Due *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h,
                                     uint16_t outline, uint16_t fill, uint16_t textcolor,
                                     char *label, uint8_t textsize)
{
    // Tweak arguments and pass to the newer initButtonUL() function...
    initButtonUL(gfx, x - (w / 2), y - (h / 2), w, h, outline, fill,
                 textcolor, label, textsize);
}

// Newer function instead accepts upper-left corner & size
void TFT_HX8357_Due_Button::initButtonUL(
                                       TFT_HX8357_Due *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h,
                                       uint16_t outline, uint16_t fill, uint16_t textcolor,
                                       char *label, uint8_t textsize)
{
    _x1           = x1;
    _y1           = y1;
    _w            = w;
    _h            = h;
    _outlinecolor = outline;
    _fillcolor    = fill;
    _textcolor    = textcolor;
    _textsize     = textsize;
    _gfx          = gfx;
    strncpy(_label, label, 12);
}

void TFT_HX8357_Due_Button::drawButton(boolean inverted) {
    uint16_t fill, outline, text;
    
    if(!inverted) {
        fill    = _fillcolor;
        outline = _outlinecolor;
        text    = _textcolor;
    } else {
        fill    = _textcolor;
        outline = _outlinecolor;
        text    = _fillcolor;
    }
    
    uint8_t r = min(_w, _h) / 4; // Corner radius
    _gfx->fillRoundRect(_x1, _y1, _w, _h, r, fill);
    _gfx->drawRoundRect(_x1, _y1, _w, _h, r, outline);
    
    _gfx->setCursor(_x1 + (_w/2) - (strlen(_label) * 3 * _textsize),
                    _y1 + (_h/2) - (4 * _textsize));
    _gfx->setTextColor(text);
    _gfx->setTextSize(_textsize);
    _gfx->print(_label);
}

boolean TFT_HX8357_Due_Button::contains(int16_t x, int16_t y) {
    return ((x >= _x1) && (x < (_x1 + _w)) &&
            (y >= _y1) && (y < (_y1 + _h)));
}

void TFT_HX8357_Due_Button::press(boolean p) {
    laststate = currstate;
    currstate = p;
}

boolean TFT_HX8357_Due_Button::isPressed() { return currstate; }
boolean TFT_HX8357_Due_Button::justPressed() { return (currstate && !laststate); }
boolean TFT_HX8357_Due_Button::justReleased() { return (!currstate && laststate); }
