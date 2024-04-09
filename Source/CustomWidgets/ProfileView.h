
#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "ProfilePicLabel.h"

class ProfileView : public QWidget{
    Q_OBJECT
public:
    ProfileView(QWidget* parent = nullptr);
    void setProfilePicture(QPixmap pixmap);
    void setName(QString name);

    QString name() const;
    QPixmap pixmap() const;

    void setNameVisible(bool visible);
private:
    QHBoxLayout* pLayout;
    ProfilePicLabel*      pProfilePicture;
    QLabel*      pName;
    const QSize iconSize = {32 , 32};
};


#endif // PROFILEVIEW_H
