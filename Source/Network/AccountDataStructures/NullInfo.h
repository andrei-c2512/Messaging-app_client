#pragma once
#include "ChatInfo.h"

class NullInfo {
public:
    static NullInfo& instance();
    ~NullInfo();
    NullInfo(const NullInfo&) = delete;
    NullInfo& operator=(const NullInfo&) = delete;

    MessageInfo& nullMessage() const noexcept;
    ChatInfo& nullChat() const noexcept;
    ContactInfo& nullContact() const noexcept;
private:
    NullInfo();
private:
    MessageInfo* pNullMessage;
    ChatInfo* pNullChat;
    ContactInfo* pNullContact;
};