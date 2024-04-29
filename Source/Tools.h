
#ifndef TOOLS_H
#define TOOLS_H
#include <QPixmap>
#include <QDebug>
#include <QImage>
#include <QDateTime>
#include <QTimeZone>
#include <QWidget>
//template<typename T>
//concept usesId = requires(T & type) {
//    type.id();
//};

class ContactInfo;
class ChatInfo;

class Tools {
public:
    //anything that's not transparent will be converted to this color
    static QImage setColorForImage(QImage newImage , QColor chroma) noexcept
    {
        for(int y = 0 ; y < newImage.height() ; y++)
            for(int x = 0 ; x < newImage.width() ; x++)
            {
                QRgb rgb = newImage.pixel(x , y);
                QColor color = QColor(rgb);
                color.setAlpha(rgb >> 24);
                if(color.alpha() != 0)
                {
                    color = chroma;
                    newImage.setPixel(QPoint(x ,y) , color.rgba());
                }
            }

        return newImage;
    }
    static QImage setColorForImage(QPixmap pixmap, QColor chroma) noexcept
    {
        return setColorForImage(pixmap.toImage() , std::move(chroma));
    }
    //returns the a list of positions of the pixels that are not transparent
    static std::vector<QPoint> getPixmapPixelList(QPixmap pixmap) noexcept
    {
        QImage newImage = pixmap.toImage();
        std::vector<QPoint> posList;
        for(int y = 0 ; y < newImage.height() ; y++)
            for(int x = 0 ; x < newImage.width() ; x++)
            {
                QRgb rgb = newImage.pixel(x , y);
                QColor color = QColor(rgb);
                color.setAlpha(rgb >> 24);

                if(color.alpha() != 0)
                    posList.emplace_back(QPoint(x , y));
            }

        return posList;
    }
    
    static std::vector<QPoint> getImagePixelList(const QImage& newImage) noexcept
    {
        std::vector<QPoint> posList;
        for (int y = 0; y < newImage.height(); y++)
            for (int x = 0; x < newImage.width(); x++)
            {
                QRgb rgb = newImage.pixel(x, y);
                QColor color = QColor(rgb);
                color.setAlpha(rgb >> 24);

                if (color.alpha() != 0)
                    posList.emplace_back(QPoint(x, y));
            }

        return posList;
    }
    static QDateTime stringToDateTime(const QString& str )
    {
        int timeStampPos = 0;
        std::vector<int> date(3);
        std::vector<int> time(3);
        int i = 0;
        QString aux;
        //ok so for some reason , when I read from the database and send it to client , the separator between date and time is sometimes 'T' and sometimes ' '
        while(str[i] != ' ')
        {
            if(str[i] == '-')
            {
                date[timeStampPos] = aux.toInt();
                timeStampPos++;
                aux = "";
            }
            else
                aux += str[i];
            i++;
        }
        date[timeStampPos] = aux.toInt();
        timeStampPos = 0;
        aux = "";
        i++;
        while(i < str.length() )
        {
            if(str[i] == ':')
            {
                time[timeStampPos] = aux.toInt();
                timeStampPos++;
                aux = "";
            }
            else
                aux += str[i];
            i++;
        }
        return QDateTime(QDate(date[0] , date[1] , date[2]) , QTime(time[0] , time[1] , time[2]) , QTimeZone(QTimeZone::LocalTime));
    }
    static std::vector<int> extractIntsFromArr(const QString& str, int start = 0)
    {
        std::vector<int> memberList;
        QString nrStr;
        //it's necessary to put such a big variable because sometimes , message from server(like chat histories) can be HUGE
        qsizetype i = str.indexOf('{' , start) + 1;

        while (str[i] != '}')
        {
            if (str[i] == ',')
            {
                memberList.emplace_back(nrStr.toInt());
                nrStr = "";
            }
            else
                nrStr += str[i];
            i++;
        }
        memberList.emplace_back(nrStr.toInt());
        return memberList;
    }
    template<typename T>
    static void insertIntoArrayWhileKeepingOrder(std::vector<T>& vec, T c)
    {
        if (vec.size() == 0)
        {
            vec.emplace_back(c); return;
        }

        int mid, left, lastLeft, right, lastRight;
        lastLeft = left = 0;
        lastRight = right = vec.size() - 1;
        /* we are finding out , through binary search , the zone in which the client could be inserted so that we can keep the order*/
        while (left < right)
        {
            mid = (left + right) / 2;
            if (vec[mid]->id() == c->id())
            {
                vec.insert(vec.begin() + mid, c);
                return;
            }
            else if (vec[mid]->id() > c->id())
            {
                lastRight = right;
                lastLeft = left;
                right = mid - 1;
            }
            else if (vec[mid]->id() < c->id())
            {
                lastLeft = left;
                lastRight = right;
                left = mid + 1;
            }
        }

        int end = std::min(lastRight + 1, (int)vec.size() - 1);
        int i = 0;
        for (i = std::max(lastLeft - 1, 0); i <= end; i++)
        {
            if (vec[i]->id() >= c->id())
            {
                vec.insert(vec.begin() + i, c); 
                return;
            }
        }
        vec.emplace_back(c);
    }
    template <typename T>
    static T binaryIdSearch(const std::vector<T>& vec, int id)
    {
        int left, right, mid;
        left = 0, right = vec.size() - 1;

        while (left <= right)
        {
            mid = (left + right) / 2;
            if (vec[mid]->id() == id)
                return vec[mid];
            else if (vec[mid]->id() < id)
                left = mid + 1;
            else if (vec[mid]->id() > id)
                right = mid - 1;
        }

        return nullptr;
    }
    template<typename T>
    static void sort(std::vector<T>& vec, std::function<bool(T , T)> op)
    {
        if (vec.size() <= 1) return;
        bool go;
        do {
            go = false;
            for(int i = 0 ; i < vec.size() - 1; i++)
                if (op(vec[i], vec[i + 1]) == false)
                {
                    T aux = vec[i];
                    vec[i] = vec[i + 1];
                    vec[i + 1] = aux;
                    go = true;
                }
        } while (go);
    }
    template<typename T> 
    static void removeItem(std::vector<T>& vec, std::function<bool(T)> cond, std::function<void(T)> deleter)
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (cond(vec[i]))
            {
                vec.erase(vec.begin() + i);
                deleter();
            }
        }
    }
    template<typename T>
    static T takeItem(std::vector<T>& vec, std::function<bool(T)> cond)
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (cond(vec[i]))
            {
                T copy = vec[i];
                vec.erase(vec.begin() + i);
                return copy;
            }
        }
        return nullptr;
    }
    static QPoint windowPos(const QWidget* widget)
    {
        QPoint pos;
        while (widget->parent() != nullptr)
        {
            pos += widget->pos();
            if (QWidget* w = qobject_cast<QWidget*>(widget->parent()))
                widget = w;
        }
        return pos;
    }
};

#endif // TOOLS_H
