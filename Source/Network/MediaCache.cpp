#include "MediaCache.h"
#include "MessageHandlingBase.h"

static QString cacheFolderName = "Cache";

MediaCache::MediaCache(QObject* parent) : QObject(parent)
{
	cacheDir = QApplication::applicationDirPath();
	cacheDir.cdUp();
	assert(cacheDir.cd(cacheFolderName));
	_count = cacheDir.count();

}
QImage MediaCache::provideImage(const QString& name) {
	QFile file(cacheDir.path() + '/' + name);
	if (file.open(QIODevice::ReadOnly) == false)
		return QImage();
	else
	{
		QByteArray arr = file.readAll();
		file.close();
		QImage img = QImage::fromData(std::move(arr), extractExtension(name).toUtf8());
		return img;
	}
}


QString MediaCache::numToString(int num)
{
	QString strNum = QString::number(num);
	if (num < 10)
		strNum.prepend('0');

	return strNum;
}

QString MediaCache::constructFileName(int count , int extension)
{
	return "f_" + numToString(count) + 'E' + QString::number(extension);
}

bool MediaCache::fileExists(const QString& name)
{
	QString filePath = cacheDir.path() + '/' + name;
	return QFile::exists(filePath);
}

QString MediaCache::extractExtension(const QString& fileName)
{
	FileExtension extension = (FileExtension)fileName.last(2).toInt();
	switch (extension)
	{
	case FileExtension::PNG:
		return "png";
	case FileExtension::JPG:
		return "jpg";
	default:
		return "";
	}
}

void MediaCache::setDirName(const QString& name)
{
	dirName = name;
	QDir dir = QApplication::applicationDirPath();
	dir.cdUp();
	dir.mkdir("cache-" + name);
	dir.cd("cache-" + name);
	cacheDir = dir;
}


void MediaCache::addImage(const QUrl& url, const QString& name)
{
	QFileInfo fileInfo(url.toString());
	
	FileExtension extension = (FileExtension)Converters::suffixType(fileInfo.suffix());

	if (extension != FileExtension::INVALID)
	{
		QString fileAbsolutePath = cacheDir.absolutePath() + "/" + name;

		QFile file = QFile(url.toString());
		if (file.open(QIODevice::ReadOnly) == false)
		{
			qDebug() << "Could not open file int media cache: " << file.errorString();
		}

		QByteArray data = file.readAll();

		file.close();

		file.setFileName(fileAbsolutePath);
		if (file.open(QIODevice::WriteOnly))
		{
			qDebug() << "!New cache image created";

			qDebug() << "Bytes written " << file.write(std::move(data));
		}
		file.close();
	}
}
