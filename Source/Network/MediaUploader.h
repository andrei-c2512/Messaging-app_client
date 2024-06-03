#pragma once
#include <QTcpSocket>
#include <queue>
#include <QImage>
#include <QUrl>
#include <QFile>
#include <QFileInfo>

struct UploadData {
	UploadData() = default;
	UploadData(QUrl url0, int chatId0, int senderId , std::vector<int> memberlist);
	QUrl url;
	int chatId;
	std::vector<int> membersList;
	int senderId;
	int handlingId;
};
class MediaUploader : public QObject {
public:
	MediaUploader(QObject* parent, QTcpSocket& socket0);
	void addMediaToQueue(const QUrl& imageUrl , int chatId  , int senderId , std::vector<int> membersList);
	void nextMedia(QString id);
	void nextChunk();
	void requestId(const QString& suf, int chatId, int senderId , std::vector<int> membersList);
	
	UploadData readyMedia(int handlingid);
private:
	static constexpr qsizetype chunkSize = 4096; // bytes // 4096
	std::queue<UploadData> mediaQueue;
	std::queue<std::unique_ptr<QByteArray>> chunkQueue;

	UploadData currentMedia;
	QString currentId;
	QTcpSocket& socket;

	// url and new file name
	std::vector<UploadData> _readyMediaList;
};