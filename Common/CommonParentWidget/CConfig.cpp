#include "CConfig.h"


CConfig::CConfig()
{

}

CConfig::~CConfig()
{

}

ConfigInformation* CConfig::getConfigInstance()
{
    return ConfigInformation::getInstance();
}

