#include "PaletteAdjuster.h"
#include "StyleRepository.h"

void PaletteAdjuster::adjustPalette(QStyleOptionButton* option) const
{
    QColor colorAux = StyleRepository::Base::backgroundColor();
    option->palette.setBrush(QPalette::ColorRole::Button , QBrush(colorAux));
    colorAux = StyleRepository::Base::textColor();
    option->palette.setBrush(QPalette::ColorRole::ButtonText , QBrush(colorAux));
    colorAux = StyleRepository::Base::borderColor();
    //option->palette.setPen(QPalette::ColorRole::Window , QBrush(colorAux));
}
void PaletteAdjuster::adjustWindowPalette(QStyleOption* option) const
{
    QColor colorAux = StyleRepository::Base::backgroundColor();
    option->palette.setBrush(QPalette::ColorRole::Window , colorAux);
}
