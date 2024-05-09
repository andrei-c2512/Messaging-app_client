#include <QImage>
#include <QPixmap>
#include <QObject>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QFileInfo>
#include <QImageReader>
#include <QImageWriter>

class MediaCache : public QObject {
public:
	MediaCache(QObject* parent = nullptr);
	QImage provideImage(const QString& name);
	QString addImage(const QUrl& url);
	bool fileExists(const QString& name);
	static QString numToString(int num);
	static QString constructFileName(int num , int extension);
private:
	std::vector<QImage> cache;
	QDir cacheDir;

	int _count;
};