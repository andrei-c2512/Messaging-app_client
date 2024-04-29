#include <QLabel>
#include "CustomStyledWidgets/CustomButton.h"
#include <QHBoxLayout>

class MediaLabel : public QLabel {
	Q_OBJECT
public:
	MediaLabel(QWidget* parent = nullptr);
	void setIndex(int index);
	void setMaxDim(int max);
	int index() const;      
private:
	void setupUi();
signals:
	void remove(int index);
private:
	QHBoxLayout* pLayout;
	CustomButton* pRemoveBtn;
	int _index;
	int _maxDim;
};