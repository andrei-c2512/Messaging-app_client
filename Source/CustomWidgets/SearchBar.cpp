#include "SearchBar.h"

SearchBar::SearchBar(QWidget* parent ) : CustomTextEdit(parent)
{
    setPlaceholderText("Search...");
    setShrinkToFit(true);
}
void SearchBar::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    //0x01000004 and 0x01000005 are the value for both enters on the keyboard
    if(toPlainText().length() > 0 && (key == 0x01000004 || key == 0x01000005))
        emit searchRequested(toPlainText());
    else
        CustomTextEdit::keyPressEvent(event);
}
void SearchBar::makeSearchRequest() { emit searchRequested(toPlainText()); }


