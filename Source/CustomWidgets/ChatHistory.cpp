#include "ChatHistory.h"
#include "StyleBase/StyleRepository.h"
#include <forward_list>
#include "Tools.h"
#include "Network/AccountDataStructures/NullInfo.h"

static QDate now = QDate::currentDate();

QSize  MessageEdit::sizeHint() const
{
    //return QSize(CustomTextEdit::sizeHint().width() , document().height;
    return QSize();
}

ChatRecord::ChatRecord(QWidget* parent) : QWidget(parent) {
    setupUi(false);
}

ChatRecord::ChatRecord(QWidget* parent, MessageInfo* pInfo0, const std::vector<QString>& memberNames, ConnectionHandler& processor, bool waitingForResponse)
{
    setupUi(waitingForResponse);
    setInfo(pInfo0, memberNames , processor);
    connect(pInfo0, &MessageInfo::outOfQueue, this, [=]() {
        pMainLayout->setStretch(0, 0);
        });
}
void ChatRecord::setupUi(bool waitingForResponse )
{
    QMargins noMargins = QMargins(0, 0, 0, 0);
    QFont chatFont = StyleRepository::Base::chatFont();

    pProfilePicture = new ProfilePicLabel;
    pProfilePicture->setPixmap1(StyleRepository::Base::defaultPfpPixmap());
    QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    policy.setRetainSizeWhenHidden(true);
    pProfilePicture->setSizePolicy(policy);

    pTime = new QLabel;
    pTime->setFont(StyleRepository::Base::informativeTextFont());

    pName = new QLabel;
    pName->setFont(chatFont);
    pName->setContentsMargins(noMargins);

    pNameAndTimeLayout = new QHBoxLayout;
    pNameAndTimeLayout->addWidget(pName);
    pNameAndTimeLayout->addWidget(pTime);
    pNameAndTimeLayout->addStretch(1);
    pNameAndTimeLayout->setContentsMargins(noMargins);


    pMessage = new CustomTextEdit;
    pMessage->setFont(chatFont);
    pMessage->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    pMessage->setReadOnly(true);
    pMessage->setShrinkToFit(true);
    pMessage->setObjectName("Message");
    pMessage->setContentsMargins(noMargins);
    pMessage->document()->setDocumentMargin(0);
    pMessage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    pUILayout = new QVBoxLayout;
    pUILayout->addLayout(pNameAndTimeLayout);
    pUILayout->addWidget(pMessage);
    pUILayout->addStretch(1);
    pUILayout->setContentsMargins(noMargins);

    pMainLayout = new QHBoxLayout(this);
    pMainLayout->addWidget(pProfilePicture, 0, Qt::AlignTop);
    pMainLayout->addLayout(pUILayout);
    pMainLayout->setContentsMargins(noMargins);

    setContentsMargins(noMargins);
}

void ChatRecord::setStatus(bool isFirstMessageOfSequence)
{
    pName->setVisible(isFirstMessageOfSequence);
    pTime->setVisible(isFirstMessageOfSequence);
    pProfilePicture->setVisible(isFirstMessageOfSequence);
}

void ChatRecord::setInfo(MessageInfo* info, const std::vector<QString>& memberNames, ConnectionHandler& processor)
{
    pInfo = info;
    connect(info, &ChatInfo::destroyed, this, [=]() {
        pInfo = &NullInfo::instance().nullMessage();
        });

    QString text = info->text();

    if (text.startsWith(MessageInfo::imageSign))
    {
        QString fileName = text.sliced(MessageInfo::imageSign.length());

        pMessage->setText("");
        pMessage->insertImage(processor.image(fileName));
        pName->setText(info->name());
    }
    else
    {
        applyTags(memberNames);
        pName->setText(info->name());
    }

    pTime->setText(dateToString(pInfo->timestamp()));
}
QString ChatRecord::dateToString(QDateTime timestamp)
{
    QDate date = timestamp.date();
    QTime time = timestamp.time();
    if (now.year() == date.year() && now.month() == date.month())
    {
        if (now.day() == date.day())
            return "Today at " + time.toString();
        else if (now.day() - date.day() == 1)
            return "Yesterday at " + time.toString();
        else
            return timestamp.toString();
    }
    else
        return timestamp.toString();
}

void ChatRecord::applyTags(const std::vector<QString>& memberNames)
{
    std::list<int> possibleTags;
    const char tagSymbol = '@';

    QString text = pInfo->text();
    int index = text.indexOf(tagSymbol), lastIndex = 0;
    //adding all occurences of tag symbol and putting as default the position 0
    while (lastIndex < index)
    {
        lastIndex = index;
        possibleTags.emplace_back(lastIndex);
        index = text.indexOf(tagSymbol, lastIndex + 1);
    }
    //if the message didn't start with tag symbol , we remove the first occurence
    if(text.length())
    if (text[0] == tagSymbol)
        possibleTags.insert(possibleTags.begin(), 0);

    std::vector<std::pair<int, QString>> lastIndexList(memberNames.size());
    //init
    for (int i = 0; i < lastIndexList.size(); i++)
        lastIndexList[i] = std::make_pair(0, memberNames[i]);

    for (auto b = lastIndexList.begin(); b != lastIndexList.end(); )
    {
        b->first = text.indexOf(b->second);
        if (b->first == -1)
            b = lastIndexList.erase(b);
        else
            b++;
    }


    for (int possiblePos : possibleTags)
    {
        for (auto b = lastIndexList.begin(); b != lastIndexList.end(); )
        {
            if (possiblePos >= b->first)
                b->first = text.indexOf(b->second, b->first + 1);
            else if (possiblePos + 1 == b->first)
            {
                text.replace(b->first - 1, b->second.size() + 1, StyleRepository::Base::applyHtmlForTag(tagSymbol + b->second));
                //text.insert(b->first + 1, StyleRepository::Base::applyHtmlForTag(b->second));
                b->first = text.indexOf(b->second, b->first + 1);
            }

            if (b->first == -1)
                b = lastIndexList.erase(b);
            else
                b++;
        }
    }
    pMessage->document()->setHtml(text);
}
ChatScrollArea::ChatScrollArea(QWidget* parent) : QScrollArea(parent)
{
    _moveScrollBar = false;
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    horizontalScrollBar()->setEnabled(false);
    QScrollBar* verticalScrollBar = QScrollArea::verticalScrollBar();

    setObjectName("ChatScrollArea");

    oldMin = verticalScrollBar->minimum(); 
    oldMax = verticalScrollBar->maximum();

    connect(verticalScrollBar, &QScrollBar::valueChanged, this, [=](int value) {
        if (value < verticalScrollBar->maximum() * 0.25f)
        emit loadMore();
    });
    connect(verticalScrollBar, &QScrollBar::rangeChanged, this, [=](int min , int max) {
        Q_UNUSED(min)
        if (_moveScrollBar == true)
        {
            verticalScrollBar->setValue(max);
            _moveScrollBar = false;
        }
        });

    connect(verticalScrollBar, &QScrollBar::rangeChanged, this, [=](int min , int max) {
        if (verticalScrollBar->value() != verticalScrollBar->maximum())
        {
            verticalScrollBar->setValue(max - oldMax + verticalScrollBar->value());
        }
        oldMax = max;
       });
    setStyleSheet(StyleRepository::Base::qSliderStyle());
    
}
void ChatScrollArea::moveScrollBarToBottom()
{
    _moveScrollBar = true;
}

void ChatScrollArea::setScrollAreaWidget(QWidget* widget) { pScrollAreaWidget = widget; }

void ChatScrollArea::resizeEvent(QResizeEvent* event)
{
    //setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());
    QScrollArea::resizeEvent(event);
}


void ChatRecord::setMessage(const QString& str) { pMessage->setText(str); }
void ChatRecord::setName(const QString& name) { pName->setText(name + ':'); }
QString ChatRecord::message() const { return pMessage->toPlainText(); }
QString ChatRecord::name() const { return pName->text(); }

ChatHistory::ChatHistory(QWidget* parent, ConnectionHandler& processor0) : QWidget(parent) , processor(processor0)
{
    lastMessageLoadedIndex = 0;
    setupUi();
  
    connect(pScrollArea, &ChatScrollArea::loadMore, this, &ChatHistory::onLoadMore);
}
void ChatHistory::setupUi()
{
    const int recordSpacing = 0;
    pScrollAreaWidget = new QWidget;
    pScrollAreaWidget->setObjectName("ListContainer");
    pHistoryLayout = new QVBoxLayout(pScrollAreaWidget);
    pHistoryLayout->addStretch(1);
    pHistoryLayout->setSpacing(recordSpacing);

    pScrollArea = new ChatScrollArea();
    pScrollArea->setWidget(pScrollAreaWidget);
    pScrollArea->setScrollAreaWidget(pScrollAreaWidget);
    pScrollArea->setWidgetResizable(true);
    QHBoxLayout* pMainLayout = new QHBoxLayout(this);
    pMainLayout->addWidget(pScrollArea);
    pInfo = nullptr;
    setObjectName("ListContainer");
}
void ChatHistory::addNRecords(int n)
{
    std::vector<ChatRecord*> newRecords(n);

    for (int i = n - 1; i >= 0; i--)
    {
        newRecords[i] = new ChatRecord(nullptr, &NullInfo::instance().nullMessage(), memberNames , processor);
        pHistoryLayout->insertWidget(0, newRecords[i]);
    }  
    recordList.insert(recordList.begin(),  newRecords.begin(), newRecords.end());
}
void ChatHistory::deleteNRecords(int n)
{
    if (n == 0) return;
    std::for_each(std::end(recordList) - n , std::end(recordList), [=](ChatRecord* record) {
        record->deleteLater();
        });
    recordList.resize(recordList.size() - n);
}


//void ChatHistory::applyTags(int begin, int end)
//{
//    if (end > recordList.size())
//        end = recordList.size();
//    for (auto b = recordList.begin() + begin; b != recordList.begin() + end; b++)
//    {
//        
//    }
//}

QSize ChatHistory::sizeHint() const
{
    return QWidget::sizeHint();
}

QSize ChatHistory::minimumSizeHint() const {
    return QWidget::sizeHint();
}

void ChatHistory::addRecord(const QString& name, const QString& message)
{
    MessageInfo* messageInfo = pInfo->addMessageToQueue(name, message);
    ChatRecord* record = new ChatRecord(nullptr, messageInfo , memberNames , processor);

    setupMessage(*messageInfo, record , recordList.back());

    recordList.emplace_back(record);
    pHistoryLayout->insertWidget(pHistoryLayout->count() - 1, record);
    pScrollArea->moveScrollBarToBottom();
}
void ChatHistory::addRecord(MessageInfo& message)
{
    ChatRecord* record = new ChatRecord(nullptr, &message, memberNames , processor);

    setupMessage(message, record , recordList.back());

    QScrollBar* scrollBar = pScrollArea->verticalScrollBar();
    bool moveToBottom = false;

    if (scrollBar->value() == scrollBar->maximum())
        moveToBottom = true;

    recordList.emplace_back(record);
    //pHistoryLayout->insertWidget(pHistoryLayout->count() - 1, record);
    pHistoryLayout->insertWidget(pHistoryLayout->count() - 1, record);
    if(moveToBottom)
        pScrollArea->moveScrollBarToBottom();
}

void ChatHistory::addMediaRecord(const QString& name, const QUrl& url)
{
    MessageInfo* messageInfo = pInfo->addMessageToQueue(name, url);
    ChatRecord* record = new ChatRecord(nullptr, messageInfo, memberNames , processor);

    setupMessage(*messageInfo, record, recordList.back());

    recordList.emplace_back(record);
    pHistoryLayout->insertWidget(pHistoryLayout->count() - 1, record);
    pScrollArea->moveScrollBarToBottom();
}

void ChatHistory::addMediaRecords(const QString& name, const std::vector<QUrl>& urls) {
    for (const QUrl& url : urls)
    {
        MessageInfo* messageInfo = pInfo->addMessageToQueue(name, url);
        ChatRecord* record = new ChatRecord(nullptr, messageInfo, memberNames , processor);

        processor.addImage(url , pInfo->id() , pInfo->members());
        setupMessage(*messageInfo, record, recordList.back());

        recordList.emplace_back(record);
        pHistoryLayout->insertWidget(pHistoryLayout->count() - 1, record);
    }
    pScrollArea->moveScrollBarToBottom();
}
void ChatHistory::setChatInfo(ChatInfo* info) {
    if (pInfo)
    {
        disconnect(pInfo, SIGNAL(newMessageAdded(MessageInfo&)), this, SLOT(addRecord(MessageInfo&)));
    }

    pInfo = info;
    memberNames = processor.storage().namesForContacts(pInfo->members());

    connect(pInfo, &ChatInfo::destroyed, this, [=]() {
        pInfo = &NullInfo::instance().nullChat();
        });
    connect(info, SIGNAL(newMessageAdded(MessageInfo&)), this, SLOT(addRecord(MessageInfo&)));
    std::vector<MessageInfo*> infoList = pInfo->lastNMessages(messagesLoaded);
    lastMessageLoadedIndex = std::min(messagesLoaded , (int)infoList.size());

    int recordDiff = int(infoList.size() - recordList.size());
    if (recordDiff > 0)
        addNRecords(recordDiff);
    else
        deleteNRecords(-recordDiff);


    pScrollArea->moveScrollBarToBottom();
    userHash.clear();
    sequence = 1;
    lastVal = -1;


    int& val = userHash[infoList[0]->name()];
    if (val == 0)
    {
        val = sequence;
        sequence++;
    }
    lastVal = val;
    recordList[0]->setInfo(infoList[0] , memberNames , processor);
    recordList[0]->setStatus(false);
    for (int i = 1; i < infoList.size(); i++)
    {
        setupMessage(*infoList[i], recordList[i], recordList[i - 1]);
        recordList[i]->setInfo(infoList[i] , memberNames , processor);
    }
    if (messagesLoaded != infoList.size())
        recordList.back()->setStatus(true);
}
void ChatHistory::onLoadMore()
{
    std::vector<MessageInfo*> infoList = pInfo->lastNMessages(lastMessageLoadedIndex , messagesLoaded);
    addNRecords(infoList.size());
    for (int i = int(infoList.size()) - 1; i >= 0; i--)
    {
        setupMessage(*infoList[i], recordList[i] , recordList[i + 1]);
        recordList[i]->setInfo(infoList[i] , memberNames , processor);
    }
    if (infoList.size() != 0)
    {
        //pScrollArea->moveScrollBarToBottom();
        lastMessageLoadedIndex += infoList.size();
    }
    else
    {
        recordList.back()->setStatus(true);
    }
}
void ChatHistory::setupMessage(MessageInfo& info, ChatRecord* record ,ChatRecord* previous)
{
    int& val = userHash[info.name()];
    if (val == 0)
    {
        val = sequence;
        sequence++;
    }

    if (val == lastVal)
    {
        record->setStatus(false);
        QMargins margins = QMargins(0, 0, 0, 0);
        record->setContentsMargins(margins);
    }
    else
    {
        previous->setStatus(true);
        lastVal = val;

        QMargins margins = QMargins(0, 30, 0, 0);
        previous->setContentsMargins(margins);
    }
    record->setInfo(&info , memberNames , processor);
}

void ChatHistory::empty()
{
    std::for_each(std::begin(recordList), std::end(recordList), [=](ChatRecord* record) {
        record->deleteLater();
        });
    recordList.resize(0);
}
int ChatHistory::chatId() const { return pInfo->id(); }

const ChatRecord& ChatHistory::recordAt(int index) { return *recordList[index]; }
