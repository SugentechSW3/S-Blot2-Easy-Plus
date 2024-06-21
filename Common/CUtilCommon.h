#ifndef CUTIL_COMMON_H
#define CUTIL_COMMON_H

//C++11
#ifdef USE_QT
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QMap>
#include <QList>
#include <QSet>
#include <QDebug>

#else
#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <stack>
#include <map>
#include <set>
#endif


#ifndef uchar
typedef unsigned char uchar ;
#endif

#ifndef ushort
typedef unsigned short ushort ;
#endif

#ifndef uint
typedef unsigned int uint ;
#endif

#ifdef USE_QT
typedef QVector<uchar> uCharArrayStream;
#else
typedef std::vector<uchar> uCharArrayStream;
//typedef std::function<void(int)> onCreated; //CallBackFunction
#endif

#define SU_PROPERTY(Type, MemberName, FaceName) \
    protected:  Type MemberName; \
    public:	Type get##FaceName() const { \
    return MemberName; \
    } \
    public: void set##FaceName(Type value) { \
    MemberName = value; \
    }

#define SU_READONLY_PROPERTY(Type, Name) __declspec( property (get = property__get_##Name) ) Type Name;\
    typedef Type property__tmp_type_##Name
#define SU_WRITEONLY_PROPERTY(Type,Name) __declspec( property (put = property__set_##Name) ) Type Name;\
    typedef Type property__tmp_type_##Name
#define GET(Name) property__tmp_type_##Name property__get_##Name()
#define SET(Name) void property__set_##Name(const property__tmp_type_##Name& value)

//타임아웃 hint : 타이머를 돌려서 지정된시간을 부여 ex) 1분 TimeOut이되면 스택에쌓인 패킷을 삭제하고 다음패킷을넘어감

#define NS_SUGENTECH_BEGIN namespace sugentech{;
#define NS_SUGENTECH_END };

#define USING_SUGENTECH using namespace sugentech;

#endif // CUTILCOMMON_H
