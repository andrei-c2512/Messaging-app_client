#include <QByteArray>
#include <qDebug>
#include <QTcpSocket>
#include "MessageHandlingBase.h"


class MediaHandler {
public:
	MediaHandler();
	bool mediaReady() const;
	QString fileName() const;
	std::unique_ptr<QByteArray> blob();
	int extension() const;
	void prepareNewMedia(QString fileName, int extension , qsizetype size);
	void addBlob(QByteArray data);
private:
	int _extension;
	std::unique_ptr<QByteArray> _blob;
	qsizetype _fileSize = 0;
	QString _fileName;
	bool _mediaReady;
};