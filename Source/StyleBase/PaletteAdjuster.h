
#ifndef PALETTEADJUSTER_H
#define PALETTEADJUSTER_H
#include <QStyleOption>

class PaletteAdjuster{
public:
    void adjustPalette(QStyleOptionButton* option) const;
    void adjustWindowPalette(QStyleOption* option) const;
};
#endif // PALETTEADJUSTER_H
