#include <QTcpSocket>
#include <queue>
#include <QImage>
#include <QUrl>
#include <QFile>
#include <QFileInfo>


class MediaUploader : public QObject {
public:
	MediaUploader(QObject* parent, QTcpSocket& socket0);
	void addMediaToQueue(const QUrl& imageUrl);
	void nextMedia(QString id);
	void nextChunk();
	void requestId(const QString& suf);
private:
	static constexpr qsizetype chunkSize = 1024; // bytes
	std::queue<QUrl> mediaQueue;
	std::queue<std::unique_ptr<QByteArray>> chunkQueue;

	QByteArray currentMedia;
	QString currentId;
	QTcpSocket& socket;

	// url and new file name
	std::list<std::pair<QUrl, QString>> readyList;
};