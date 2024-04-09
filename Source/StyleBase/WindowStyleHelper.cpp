#include "WindowStyleHelper.h"
#include "StyleRepository.h"

void WindowStyleHelper::draw(const QStyleOption* styleOpt , QPainter* painter) const
{
    painter->setBrush(StyleRepository::Base::backgroundColor());
    painter->drawRect(styleOpt->rect);
}
