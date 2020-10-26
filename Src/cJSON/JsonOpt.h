#ifndef JSONOPT_H
#define JSONOPT_H

#include "cJSON.h"
#include <string>


class JsonOpt
{
private:
    cJSON * m_pJson;

    void ReleaseJson();
public:
    JsonOpt();
    JsonOpt(const cJSON * _pcJson);
    JsonOpt(const JsonOpt & _pJo);
    JsonOpt(const std::string _szJsonContent);
    ~JsonOpt();

    void Clear(void);
    void SetArray(); //设为json数组
    bool IsArray();
    bool IsNull();
    int ArraySize();
    void LoadFromString(const std::string _szJsonContent);
    std::string ToString();
    int ToInt();
    bool ToBool();
    void AddItem(const std::string _szKey,const std::string _szValue);
    void AddItem(const std::string _szKey,const int _sValue);
    void AddItem(const std::string _szKey,const bool _bValue);
    void AddItem(const std::string _szKey,const JsonOpt & _oValue);
    void AddArrayItem(const std::string _szValue);
    void AddArrayItem(const int _sValue);
    void AddArrayItem(const bool _bValue);
    void AddArrayItem(const JsonOpt & _oValue);
    void RemoveArrayItem(const int index);
    void ReplaceItem(const std::string _szKey,const std::string _szNewItem);
    void ReplaceItem(const std::string _szKey,const int _sNewItem);
    void ReplaceItem(const std::string _szKey,const bool _bNewItem);
    void ReplaceItem(const std::string _szKey,const JsonOpt & _oNewItem);
    JsonOpt NextItemInArray(JsonOpt & _oFron);
    JsonOpt operator[](const std::string _szSubItem);
    JsonOpt operator[](const int _index);
    JsonOpt & operator=(const JsonOpt & _oJsonOpt);
    JsonOpt & operator=(const cJSON * _pcJson);
};

#define ForEachArrayItem(Item,Array) JsonOpt Item = (Array.ArraySize()>0?Array[0]:JsonOpt(nullptr));for(int index = 0;index<Array.ArraySize();++index,Item=Array[index])

#endif // JSONOPT_H
