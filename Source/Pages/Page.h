
#ifndef PAGE_H
#define PAGE_H
#include "Network/ServerInfoProcessor.h"
#include <QWidget>

class Page : public QWidget{
    Q_OBJECT
public:
    Page(QWidget* parent , ServerInfoProcessor& pServerInfoProcessor0);
protected:
    ServerInfoProcessor& serverInfoProcessor;
};

#endif // PAGE_H
