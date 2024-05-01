#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QWidget>
#include <QEvent>
#include <qpalette.h>

class KeywordLabel : public QLabel {
public:
	KeywordLabel(QWidget* parent = nullptr);
	void setSelected(bool selected);
protected:
	bool event(QEvent* ev) override;
private:
	bool _selected;
	QColor _selectedColor;
	QColor _unselectedColor;
};
class KeywordCombo : public QWidget {
	Q_OBJECT
public:
	KeywordCombo(QWidget* parent = nullptr);
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
	void setTextEdit(const QTextEdit* edit);
	QString selectedString() const;
public:
	void goUp();
	void goDown();
	void setSelected(int index);
public slots:
	void setKeywords(std::vector<QString> list);
	void hide();
private:
	void setupUi();
private:
	QVBoxLayout* pLayout;
	std::vector<KeywordLabel*> keyWordLabel_list;
	std::vector<QString> keyWord_list;
	QLabel* pEmptyLabel;

	int selectedIndex;
	const QTextEdit* edit;
};