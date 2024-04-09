#include "WarningLabel.h"

WarningLabel::WarningLabel(QWidget* parent)
    :QLabel(parent)
{
    pEffect = new QGraphicsOpacityEffect;
    pEffect->setOpacity(1.0f);

    QLabel::setGraphicsEffect(pEffect);
    QLabel::setStyleSheet("color: red;");
    QLabel::setText("This field is obligatory");
    setState(true);
}

//void WarningLabel::setStyleSheet1(QString str)
//{
//    int styleBegin = str.indexOf( "*{");
//
//    //checking if the style has been changed hover only
//    if(styleBegin == -1)
//    {
//        QLabel::setStyleSheet(std::move(str));
//    }
//    else
//    {
//        int txtColorBegin = str.indexOf("color:" , styleBegin);
//        //checking if the NOT hover only style has color change
//        if(txtColorBegin == -1)
//        {
//            QLabel::setStyleSheet(std::move(str));
//        }
//        else
//        {
//            //now we are gonna extract that color from styleSheet
//            int colorBegin = str.indexOf(':' , txtColorBegin);
//            int colorEnd = str.indexOf(';' , colorBegin);
//
//            QString colorInString;
//            for(int ind = colorBegin + 1 ; ind < colorEnd ; ind++)
//            {
//                colorInString += str[ind];
//            }
//            //extracting the values from the string
//            std::vector<int> rgb(3,0);
//            int valIndex = 0;
//            QString aux;
//            for(int ind = colorInString.indexOf('(') + 1; ind < colorInString.indexOf(')') ; ind++)
//            {
//                if(colorInString[ind] == ',')
//                {
//                    rgb[valIndex] = aux.toInt();
//                    aux.clear();
//                    valIndex++;
//                }
//                else
//                {
//                    aux += colorInString[ind];
//                }
//            }
//
//            _textColor = QColor(rgb[0] , rgb[1] , rgb[2]);
//
//            //now we check if the text is active. If it isn't , then the text must be transparent
//        }
//    }
//}
void WarningLabel::setState(bool state)
{
    _active = std::move(state);
    if(_active)
        pEffect->setOpacity(1.0f);
    else
        pEffect->setOpacity(0.0f);
}
void WarningLabel::addWarningCondition(std::function<bool()> cond)
{
    warningCondtionList.emplace_back(std::move(cond));
    checkCondition();
}
void WarningLabel::setWarningCondtions(std::vector<std::function<bool()>> list)
{
    warningCondtionList = std::move(list);
}
void WarningLabel::checkCondition()
{
    for(std::function<bool()>& func : warningCondtionList)
    {
        if(func())
        {
            setState(true); return;
        }
    }
    setState(false);
}
bool WarningLabel::active() const{  return _active;}
void WarningLabel::setText(QString str) { QLabel::setText(std::move(str)); setState(true);}
