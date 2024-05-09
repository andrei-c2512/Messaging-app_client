#include "MediaUploader.h"
#include "MessageHandlingBase.h"

MediaUploader::MediaUploader(QObject* parent, QTcpSocket& socket0) : QObject(parent) , socket(socket0)
{
}
void MediaUploader::addMediaToQueue(const QUrl& imageUrl)
{
	bool wasEmpty = mediaQueue.size() == 0;
	mediaQueue.emplace(imageUrl);
	QFileInfo info(imageUrl.toString());

	if(wasEmpty)
		requestId(info.suffix());
}
void MediaUploader::nextMedia(QString id)
{
	if (!mediaQueue.empty())
	{
		currentId = id;

		QUrl url = mediaQueue.front();
		QFile file(url.toString());

		file.open(QFile::ReadOnly);

		while (file.atEnd() == false)
		{
			chunkQueue.emplace(std::make_unique<QByteArray>(file.read(chunkSize)));
		}

		mediaQueue.pop();
		nextChunk();
	}
}

void MediaUploader::nextChunk()
{
	if (chunkQueue.empty() == false)
	{
		std::unique_ptr<QByteArray> data = std::move(chunkQueue.front());
		if (chunkQueue.size() == 1)
		{
			data->append('1');
			qDebug() << "~~Uploading final chunk...";
		}
		else
		{
			data->append('0');
			qDebug() << "~~Uploading chunk...";
		}

		chunkQueue.pop();
		QByteArray message = QByteArray::number((int)RequestToServer::MediaChunk) + '(' + currentId.toUtf8() + ',' + *std::move(data) + ')';
		socket.write(message);
		socket.flush();
	}

	if (chunkQueue.empty() == true && mediaQueue.size())
	{
		QUrl url = mediaQueue.front();
		QFile file(url.toString());
		QString extension = QFileInfo(file).suffix();
		requestId(extension);
	}

}

void MediaUploader::requestId(const QString& suf)
{
	qDebug() << "~~Requesting next ID for media...";
	socket.write(QByteArray::number((int)RequestToServer::MediaUploadId) + '(' + QByteArray::number(Converters::suffixType(suf)) + ')');
}