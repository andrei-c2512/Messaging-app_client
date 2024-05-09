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
	QImage img = QImageReader(cacheDir.absolutePath() + '/' + name).read();
	return img;
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

QString MediaCache::addImage(const QUrl& url)
{
	QFileInfo fileInfo(url.toString());
	
	FileExtension extension = (FileExtension)Converters::suffixType(fileInfo.suffix());

	if (extension == FileExtension::INVALID)
		return "";
	else
	{
		QString fileName = constructFileName(_count, (int)extension);
		QString fileAbsolutePath = cacheDir.absolutePath() + fileName;

		QFile file(fileAbsolutePath);
		if (file.open(QIODevice::ReadOnly))
			qDebug() << "!New cache image created";
		file.close();

		QImage image = QImageReader(url.toString()).read();
		QImageWriter writer(fileAbsolutePath);
		writer.write(image);
		return fileName;
	}
}
