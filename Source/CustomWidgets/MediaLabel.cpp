#include "MediaLabel.h"
#include "StyleBase/ButtonStyleRepository.h"

MediaLabel::MediaLabel(QWidget* parent) : QLabel(parent)
{
	setupUi();
	_maxDim = 200;
	setMaximumHeight(_maxDim);
	setMaximumWidth(_maxDim);
}

void MediaLabel::setupUi()
{
	pRemoveBtn = new CustomButton(this , ButtonStyleRepository::rejectButton());
	connect(pRemoveBtn, &CustomButton::clicked, this, [=]() {
		emit remove(_index);
		});
	pLayout = new QHBoxLayout(this);

	pLayout->addWidget(pRemoveBtn, 0, Qt::AlignRight | Qt::AlignTop);
}
void MediaLabel::setMaxDim(int max) { 
	_maxDim = max; 
	setMaximumHeight(_maxDim);
	setMaximumWidth(_maxDim);
}
void MediaLabel::setIndex(int index) { _index = index; }
int MediaLabel::index() const { return _index; }
