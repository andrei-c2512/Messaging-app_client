
#ifndef SEARCHBAR_H
#define SEARCHBAR_H
#include "CustomTextEdit.h"

class SearchBar : public CustomTextEdit{
    Q_OBJECT
public:
    SearchBar(QWidget* parent = nullptr);
public slots:
    void makeSearchRequest();
signals:
    void searchRequested(const QString& message);
protected:
    void keyPressEvent(QKeyEvent *event) override;
};


#endif // SEARCHBAR_H
