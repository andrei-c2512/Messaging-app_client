#include "MediaUploader.h"
#include "MessageHandlingBase.h"
#include "Tools.h"

UploadData::UploadData(QUrl url0, int chatId0,  int senderId0 ,std::vector<int> memberlist)
	:url(url0) , chatId(chatId0) , membersList(memberlist) , senderId(senderId0)
{}


MediaUploader::MediaUploader(QObject* parent, QTcpSocket& socket0) : QObject(parent) , socket(socket0)
{
}
void MediaUploader::addMediaToQueue(const QUrl& imageUrl, int chatId,  int senderId , std::vector<int> membersList)
{
	bool wasEmpty = mediaQueue.size() == 0;
	mediaQueue.emplace(UploadData(imageUrl , chatId , senderId , membersList));
	QFileInfo info(imageUrl.toString());

	if(wasEmpty)
		requestId(info.suffix() , chatId , senderId ,  membersList );
}
void MediaUploader::nextMedia(QString id)
{
	if (!mediaQueue.empty())
	{
		currentId = id;

		currentMedia = mediaQueue.front();
		currentMedia.handlingId = id.toInt();
		QFile file(currentMedia.url.toString());

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
			_readyMediaList.emplace_back(currentMedia);
		}
		else
		{
			data->append('0');
			qDebug() << "~~Uploading chunk...";
		}

		chunkQueue.pop();
		QByteArray message = QByteArray::number((int)RequestToServer::UploadMediaChunk) + '(' + currentId.toUtf8() + ',' + *std::move(data) + ')';
		socket.write(message);
		socket.flush();
	}

	if (chunkQueue.empty() == true && mediaQueue.size())
	{
		UploadData data = mediaQueue.front();
		QFile file(data.url.toString());
		QString extension = QFileInfo(file).suffix();
		requestId(extension , data.chatId  , data.senderId , data.membersList);
	}

}

UploadData MediaUploader::readyMedia(int handlingId)
{
	for (auto b = _readyMediaList.begin(); b != _readyMediaList.end(); b++)
	{
		if (b->handlingId == handlingId)
		{
			UploadData copy = *b;
			_readyMediaList.erase(b);
			return copy;
		}
	}
	qDebug() << "DID NOT FIND THE HANDLING ID";
	return UploadData();
}

void MediaUploader::requestId(const QString& suf, int chatId, int senderId , std::vector<int> membersList)
{
	for (int i = 0; i < membersList.size(); i++)
	{
		if (membersList[i] == senderId)
		{
			membersList.erase(membersList.begin() + i);
			break;
		}
	}

	qDebug() << "~~Requesting next ID for media...";
	std::vector<QByteArray> args = { QByteArray::number(Converters::suffixType(suf))  , QByteArray::number(chatId)  ,
		QByteArray::number(senderId) , Tools::intArrToByteArr(',' , membersList) };

	QByteArray cmd = Tools::makeList('(', ')', ',', std::move(args));
	qDebug() << cmd;
	socket.write(QByteArray::number((int)RequestToServer::MediaUploadId) + cmd);
}