#include "JsonOpt.h"
#include "../Log/Log.h"

JsonOpt::JsonOpt()
{
    m_pJson = nullptr;
}

JsonOpt::JsonOpt(const cJSON * _pcJson)
{
    if(cJSON_IsNull(_pcJson) == false)
        m_pJson = cJSON_Duplicate(_pcJson,true);
    else
        m_pJson = nullptr;
}

JsonOpt::JsonOpt(const JsonOpt & _pJo)
    :JsonOpt(_pJo.m_pJson)
{
}

JsonOpt::JsonOpt(const std::string _szJsonContent)
{
    m_pJson = nullptr;
    LoadFromString(_szJsonContent);
}

JsonOpt::~JsonOpt()
{
    ReleaseJson();
}

void JsonOpt::Clear()
{
    ReleaseJson();
}

void JsonOpt::SetArray()
{
    ReleaseJson();
    m_pJson = cJSON_CreateArray();
}

void JsonOpt::ReleaseJson()
{
    if(m_pJson != nullptr)
    {
        cJSON_Delete(m_pJson);
        m_pJson = nullptr;
    }
}

bool JsonOpt::IsArray()
{
    if(IsNull() == true)
        return false;
    return cJSON_IsArray(m_pJson)?true:false;
}

bool JsonOpt::IsNull()
{
    return m_pJson==nullptr?true:false;
}

int JsonOpt::ArraySize()
{
    if(IsNull() == false)
        if(IsArray())
            return cJSON_GetArraySize(m_pJson);
    return 0;
}

void JsonOpt::LoadFromString(const std::string _szJsonContent)
{
    ReleaseJson();
    if(_szJsonContent.empty() == false)
    {
        m_pJson = cJSON_Parse(_szJsonContent.c_str());
    }
    else
        m_pJson = nullptr;
}

std::string JsonOpt::ToString()
{
    if(IsNull() == true)
        return "";
    if(cJSON_IsString(m_pJson))
        return m_pJson->valuestring;
    char * pContent = cJSON_PrintUnformatted(m_pJson);
    std::string szContent = pContent;
    free(pContent);
    return szContent;
}

int JsonOpt::ToInt()
{
    if(IsNull() == true)
        return 0;
    return m_pJson->valueint;
}

bool JsonOpt::ToBool()
{
    return ToInt()?true:false;
}


void JsonOpt::AddItem(const std::string _szKey, const std::string _szValue)
{
    if(IsNull() == true)
        m_pJson = cJSON_CreateObject();
    cJSON_AddStringToObject(m_pJson,_szKey.c_str(),_szValue.c_str());
}

void JsonOpt::AddItem(const std::string _szKey, const int _sValue)
{
    if(IsNull() == true)
        m_pJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(m_pJson,_szKey.c_str(),_sValue);
}
void JsonOpt::AddItem(const std::string _szKey, const bool _bValue)
{
    if(IsNull() == true)
        m_pJson = cJSON_CreateObject();
    cJSON_AddBoolToObject(m_pJson,_szKey.c_str(),_bValue);
}

void JsonOpt::AddItem(const std::string _szKey, const JsonOpt &_oValue)
{
    if(IsNull() == true)
        m_pJson = cJSON_CreateObject();
    cJSON_AddItemToObject(m_pJson,_szKey.c_str(),cJSON_Duplicate(_oValue.m_pJson,true));
}

void JsonOpt::AddArrayItem(const std::string _szValue)
{
    if(IsNull())
        SetArray();
    if(IsArray())
    {
        cJSON_AddItemToArray(m_pJson,cJSON_CreateString(_szValue.c_str()));
    }
}

void JsonOpt::AddArrayItem(const int _sValue)
{
    if(IsNull())
        SetArray();
    if(IsArray())
    {
        cJSON_AddItemToArray(m_pJson,cJSON_CreateNumber(_sValue));
    }
}

void JsonOpt::AddArrayItem(const bool _bValue)
{
    if(IsNull())
        SetArray();
    if(IsArray())
    {
        cJSON_AddItemToArray(m_pJson,cJSON_CreateBool(_bValue));
    }
}

void JsonOpt::AddArrayItem(const JsonOpt &_oValue)
{
    if(IsNull())
        SetArray();
    if(IsArray())
    {
        cJSON_AddItemToArray(m_pJson,cJSON_Duplicate(_oValue.m_pJson,true));
    }
}

void JsonOpt::RemoveArrayItem(const int index)
{
   if(ArraySize() > index)
   {
       cJSON_DetachItemFromArray(m_pJson,index);
   }
}

void JsonOpt::ReplaceItem(const std::string _szKey, const std::string _szNewItem)
{
    if(IsNull())
        return;
    cJSON_ReplaceItemInObject(m_pJson,_szKey.c_str(),cJSON_CreateString(_szNewItem.c_str()));
}
void JsonOpt::ReplaceItem(const std::string _szKey, const int _sNewItem)
{
    if(IsNull())
        return;
    cJSON_ReplaceItemInObject(m_pJson,_szKey.c_str(),cJSON_CreateNumber(_sNewItem));
}

void JsonOpt::ReplaceItem(const std::string _szKey, const bool _bNewItem)
{
    if(IsNull())
        return;
    cJSON_ReplaceItemInObject(m_pJson,_szKey.c_str(),cJSON_CreateBool(_bNewItem));
}

void JsonOpt::ReplaceItem(const std::string _szKey, const JsonOpt &_oNewItem)
{
    if(IsNull())
        return;
    cJSON_ReplaceItemInObject(m_pJson,_szKey.c_str(),cJSON_Duplicate(_oNewItem.m_pJson,true));
}

JsonOpt JsonOpt::operator[](const std::string _szSubItem)
{
    if(IsNull() == true)
        return JsonOpt();
    return JsonOpt(cJSON_GetObjectItem(m_pJson,_szSubItem.c_str()));
}

JsonOpt JsonOpt::operator[](const int _index)
{
    if(IsArray())
    {
        return JsonOpt(cJSON_GetArrayItem(m_pJson,_index));
    }
    return JsonOpt();
}

JsonOpt & JsonOpt::operator=(const JsonOpt &_oJsonOpt)
{
    ReleaseJson();
    m_pJson = cJSON_Duplicate(_oJsonOpt.m_pJson,true);
    return *this;
}

JsonOpt & JsonOpt::operator=(const cJSON *_pcJson)
{
    ReleaseJson();
    m_pJson = cJSON_Duplicate((cJSON *)_pcJson,true);
    return *this;
}





