#include "CustomTextEdit.h"
#include <map>
#include <QLabel>
#include "KeywordCombo.h"

class TextEditWidget :public QWidget{
public:
	TextEditWidget(QWidget* parent = nullptr);
	void setText(const QString& str);
	QString text() const;
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
protected:
	void paintEvent(QPaintEvent* ev);
private:
	QString _text;

};

class STextEdit : public CustomTextEdit {
	Q_OBJECT
public:
	enum WritingResult {
		Null , Contains , Match
	};
public:
	STextEdit(QWidget* parent  , KeywordCombo& combo);
	void setWordsForKey(Qt::Key key, std::vector<QString> specialWords);
	void addNewCollection(Qt::Key key , std::vector<QString> specialWords);
	void clearCollection();
signals:
	void newList(const std::vector<QString>& list);
	void keywordDiscontinued();
protected:
	void keyPressEvent(QKeyEvent* event) override;
	WritingResult isWritingAKeyWord(Qt::Key key , QString str) const;
	std::vector<QString> matchingKeywords(Qt::Key key ,const QString& str) const;
	void mousePressEvent(QMouseEvent* event) override;
	void insertKeyword(QString str);
private:
	std::map<Qt::Key, std::vector<QString>> map;
	bool _possibleKeyWordStarted;
	bool _keywordCreated;
	qint16 _startPos;
	Qt::Key _currentSpecialKey;
	QChar _lastChar;
	QTextCharFormat normalFormat;
	KeywordCombo& comboWidget;
};
