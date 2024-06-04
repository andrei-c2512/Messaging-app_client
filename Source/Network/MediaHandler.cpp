#include "MediaHandler.h"

MediaHandler::MediaHandler()
{

}
bool MediaHandler::mediaReady() const { return _mediaReady; }
QString MediaHandler::fileName() const { return _fileName; }
int MediaHandler::extension() const { return _extension; }
std::unique_ptr<QByteArray> MediaHandler::blob()
{
	_mediaReady = false;
	return std::move(_blob);
}
void MediaHandler::prepareNewMedia(QString fileName, int extension, qsizetype size)
{
	_blob = std::make_unique<QByteArray>();
	_extension = extension;
	_fileName = fileName;
	_fileSize = size;
}
void MediaHandler::addBlob(QByteArray data)
{
	char last = data.back();
	bool finalChunk = last == '1';
	data.removeLast();

	_blob->append(std::move(data));
	if (finalChunk)
		_mediaReady = true;
}