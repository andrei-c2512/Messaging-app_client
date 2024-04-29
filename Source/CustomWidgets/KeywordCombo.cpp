#include "KeywordCombo.h"
#include "StyleBase/StyleRepository.h"
#include "Tools.h"

KeywordLabel::KeywordLabel(QWidget* parent) : QLabel(parent) {
	setAttribute(Qt::WA_Hover);
	_selected = false;
}
void KeywordLabel::setSelected(bool selected)
{
	QPalette p = palette();
	_selected = selected;
	if (_selected)
		//rgb(35, 53, 67)
		//#41BF9B
		setStyleSheet("background-color: #41BF9B;");
	else
		setStyleSheet("background-color: rgb(35, 53, 67);");
}

bool KeywordLabel::event(QEvent* ev)
{
	switch (ev->type())
	{
	case QEvent::HoverEnter:
		setSelected(true);
		break;
	case QEvent::HoverLeave:
		setSelected(false);
		break;
	}
	return QWidget::event(ev);
}

KeywordCombo::KeywordCombo(QWidget* parent) : QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setAttribute(Qt::WA_StyleSheet);
	setupUi();
	edit = nullptr;
}
void KeywordCombo::setKeywords(std::vector<QString> list)
{
	selectedIndex = 0;
	keyWord_list = list;
	short dif = short(keyWordLabel_list.size()) - short(keyWord_list.size());
	short oldSize = (short)keyWordLabel_list.size();
	if (dif < 0)
	{
		dif = -dif;
		keyWordLabel_list.resize(oldSize + dif);
		auto e = keyWordLabel_list.end();
		for (auto b = keyWordLabel_list.begin() + oldSize; b != e; ++b)
		{
			*b = new KeywordLabel;
			pLayout->insertWidget(pLayout->count() - 1, *b);
		}

	}
	else if (dif > 0)
	{
		for (short i = oldSize - dif; i < oldSize; i++)
			keyWordLabel_list[i]->setVisible(false);
	}
	for (int i = 0; i < keyWord_list.size(); i++)
	{
		keyWordLabel_list[i]->setText(keyWord_list[i]);
		keyWordLabel_list[i]->setVisible(true);
		keyWordLabel_list[i]->setObjectName("KeywordLabel");
	}
	setVisible(true);
	QSize sizeH  (sizeHint());
	setGeometry(QRect(Tools::windowPos(edit) - QPoint(0 , sizeH.height()), std::move(sizeH)));
	keyWordLabel_list[selectedIndex]->setSelected(true);
}
void KeywordCombo::goDown() {
	if (keyWord_list.size() == 0) return;
	keyWordLabel_list[selectedIndex]->setSelected(false);
	selectedIndex++;
	if (selectedIndex == keyWord_list.size())
		selectedIndex = 0;
	keyWordLabel_list[selectedIndex]->setSelected(true);
}
void KeywordCombo::goUp()
{
	if (keyWord_list.size() == 0) return;

	keyWordLabel_list[selectedIndex]->setSelected(false);
	selectedIndex--;
	if (selectedIndex == -1)
		selectedIndex = keyWord_list.size() - 1;
	keyWordLabel_list[selectedIndex]->setSelected(true);
}

void KeywordCombo::setupUi()
{
	pEmptyLabel = new QLabel;
	pEmptyLabel->setFont(StyleRepository::Base::standardFont());
	pEmptyLabel->setText("No results");
	pLayout = new QVBoxLayout(this);
}
QString KeywordCombo::selectedString() const { return keyWord_list[selectedIndex]; }
void KeywordCombo::hide() { setVisible(false); }

QSize KeywordCombo::sizeHint() const
{
	if (edit)
		return QSize(edit->width(), QWidget::sizeHint().height());
	else
		return QWidget::sizeHint();
}
QSize KeywordCombo::minimumSizeHint() const{ return sizeHint();}
void KeywordCombo::setTextEdit(const QTextEdit* edit0) { edit = edit0; }
