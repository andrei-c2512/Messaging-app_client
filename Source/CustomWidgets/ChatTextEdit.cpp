#include "ChatTextEdit.h"
#include "StyleBase/StyleRepository.h"

TextEditWidget::TextEditWidget(QWidget* parent) : QWidget(parent)
{
}
QSize TextEditWidget::sizeHint() const {
	QFontMetrics metrics = fontMetrics();
	QMargins margins = contentsMargins();
	return QSize(margins.left() + margins.right() + metrics.horizontalAdvance(_text), margins.top() + margins.bottom() + metrics.height());
}
QSize TextEditWidget::minimumSizeHint() const { return sizeHint(); }

void TextEditWidget::setText(const QString& str) { _text = str; }
QString TextEditWidget::text() const { return _text; }
void TextEditWidget::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.setPen("black");
	painter.setFont(font());

	painter.drawText(rect(), Qt::AlignCenter, _text);
}

STextEdit::STextEdit(QWidget* parent, KeywordCombo& combo) : CustomTextEdit(parent) ,comboWidget(combo)
{
	setTextInteractionFlags(textInteractionFlags() | Qt::LinksAccessibleByMouse);
	_possibleKeyWordStarted = false;
	_startPos = 0;
	_currentSpecialKey = Qt::Key_0;
	_lastChar = ' ';
	_keywordCreated = false;
}
void STextEdit::setWordsForKey(Qt::Key key, std::vector<QString> specialWords)
{
	if (map.count(key) == 0) return;
	map[key] = specialWords;
}
void STextEdit::addNewCollection(Qt::Key key, std::vector<QString> specialWords)
{
	map[key] = specialWords;
}
void STextEdit::clearCollection() {
	map.clear(); 
	_startPos = 0;
	_currentSpecialKey = Qt::Key_0;
	_lastChar = ' ';
}

void STextEdit::keyPressEvent(QKeyEvent* event) 
{
	Qt::Key key = (Qt::Key)event->key();
	if (key == Qt::Key::Key_Up)
	{
		comboWidget.goUp(); return;
	}
	else if (key == Qt::Key::Key_Down)
	{
		comboWidget.goDown(); return;
	}

	if (_keywordCreated && Qt::Key::Key_Tab == key)
	{
		QTextCursor cursor = textCursor();
		cursor.select(QTextCursor::WordUnderCursor);
		QString string = QKeySequence(_currentSpecialKey).toString() + cursor.selectedText();

		insertKeyword(string);

		_keywordCreated = false;
		_possibleKeyWordStarted = false;
		emit keywordDiscontinued();
		return;
	}
	else if ((key == Qt::Key::Key_Enter || Qt::Key::Key_Tab == key) && _possibleKeyWordStarted)
	{
		QTextCursor cursor = textCursor();
		QTextCharFormat linkFormat, original;
		setTextCursor(cursor);

		QString string = QKeySequence(_currentSpecialKey).toString() + comboWidget.selectedString();
		insertKeyword(string);
		emit keywordDiscontinued();
		_possibleKeyWordStarted = false;
		_keywordCreated = false;
		return;
	}
	else
	{
		_keywordCreated = false;
	}

	QString str = toPlainText();
	bool matched = false;
	QTextCharFormat original = textCursor().charFormat();

	if (key != Qt::Key_Backspace)
	{
		if (_possibleKeyWordStarted == false)
		{
			if (map.count(key) == 1)
			{
				_startPos = toPlainText().length();
				_possibleKeyWordStarted = true;
				_currentSpecialKey = key;
				emit newList(map[key]);
			}
		}
		else if (_possibleKeyWordStarted == true)
		{
			QString oldPortion = toPlainText().mid(_startPos + 1);
			QString newPortion = oldPortion + event->text();
			if (oldPortion.size() != newPortion.size())
			{
				switch (isWritingAKeyWord(_currentSpecialKey, newPortion))
				{
				case WritingResult::Null:
					emit keywordDiscontinued();
					break;
				case WritingResult::Contains:
					emit newList(matchingKeywords(_currentSpecialKey, newPortion));
					break;
				case WritingResult::Match:
					_keywordCreated = true;
					break;
				}
			}
		}
	}
	else 
	{	
		if (str.length())
		{
			if (_possibleKeyWordStarted)
			{
				if (str.back() == str[_startPos])
				{
					emit keywordDiscontinued();
					_possibleKeyWordStarted = false;
					_startPos = -1;
				}
				else
				{
					QTextCursor cursor = textCursor();
					cursor.select(QTextCursor::WordUnderCursor);
					QTextCharFormat format = cursor.charFormat();
					QString str = cursor.selectedText();
					str.removeLast();
					emit newList(matchingKeywords(_currentSpecialKey, str));
				}
			}
			else
			{
				QTextCursor cursor = textCursor();
				cursor.select(QTextCursor::WordUnderCursor);
				QTextCharFormat format = cursor.charFormat();
				if (format.isAnchor())
				{
					cursor.deleteChar();
				}
			}
		}
	}


	CustomTextEdit::keyPressEvent(event);
}

void STextEdit::insertKeyword(QString string)
{
	QTextCursor cursor = textCursor();
	QTextCharFormat linkFormat, original;
	setTextCursor(cursor);

	cursor.setPosition(_startPos + 1);
	cursor.select(QTextCursor::WordUnderCursor);

	original = cursor.charFormat();

	cursor.deleteChar();
	cursor.deletePreviousChar();

	cursor.insertHtml("<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<style> "
		".rounded-light-blue{"
		"	background-color: #16c172;"
		"	border-radius: 2px;"
		"	display: inline-block;"
		"}"
		"</style>"
		"	</head>"
		"	<body>"
		"	<!--Use the class in a span element to style the word \"floppa\" -->"
		"	<span class=\"rounded-light-blue\"><a href = \"www.google.com\">" + string + "</a></span>"
		"	</body>"
		"	</html>");
	cursor.setPosition(cursor.selectionEnd());
	cursor.setCharFormat(original);
}

STextEdit::WritingResult STextEdit::isWritingAKeyWord(Qt::Key key, QString str) const
{
	const std::vector<QString>& list = map.at(key);
	for (const QString& keyword : list)
	{
		if (keyword.startsWith(str , Qt::CaseInsensitive))
		{
			if (keyword.length() == str.length())
				return STextEdit::WritingResult::Match;
			else
				return STextEdit::WritingResult::Contains;
		}
	}
	return STextEdit::WritingResult::Null;
}

std::vector<QString> STextEdit::matchingKeywords(Qt::Key key ,const QString& str) const
{
	if (str.length() == 0) return map.at(key);

	std::vector<QString> matchingList;
	const std::vector<QString>& list = map.at(key);
	for (const QString& keyword : list)
	{
		if (keyword.startsWith(str, Qt::CaseInsensitive))
		{
			matchingList.emplace_back(keyword);
		}
	}
	return matchingList;
}
void STextEdit::mousePressEvent(QMouseEvent* event)
{
	QTextCursor cursor = cursorForPosition(event->pos());
	CustomTextEdit::mousePressEvent(event);
}
