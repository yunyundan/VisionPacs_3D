#include "stdafx.h"
#include "AppConfig.h"

vector<MODINFO> CAppConfig::m_ModInfoV;

CAppConfig::CAppConfig()
{
}


CAppConfig::~CAppConfig()
{
}

bool CAppConfig::GetBaseConfig()
{
	QString sAppPath = QCoreApplication::applicationDirPath();
	QString sFilePath = sAppPath + "//VisionPacs.xml";
	QFile file(sFilePath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{		
		return false;
	}

	QXmlStreamReader xConfig(&file);
	MODINFO *modInfo = NULL;
	INFOITEM *itemInfo = NULL;
	while (!xConfig.atEnd() && !xConfig.hasError())
	{
		xConfig.readNext();
		if (xConfig.isStartElement())
		{
			QXmlStreamAttributes attr = xConfig.attributes();
			if (xConfig.name() == "Modality")
			{	
				if (modInfo)
				{
					delete modInfo;
					modInfo = NULL;
				}
					
				modInfo = new MODINFO;
				QXmlStreamAttributes attributes = xConfig.attributes();
				if (attributes.hasAttribute("name"))
				{
					modInfo->sModality = attributes.value("name").toString();
				}
			}

			if (xConfig.name() == "Font")
			{
				QXmlStreamAttributes attributes = xConfig.attributes();
				if (attributes.hasAttribute("face"))
				{
					modInfo->sFaceName = attributes.value("face").toString();
				}
				if (attributes.hasAttribute("size"))
				{
					modInfo->nSize = attributes.value("size").toString().toInt();
				}
				if (attributes.hasAttribute("color"))
				{
					modInfo->clor = attributes.value("color").toString().toInt();
				}
			}
			
			if (xConfig.name() == "Element")
			{
				if (itemInfo)
				{
					delete itemInfo;
					itemInfo = NULL;
				}
				itemInfo = new INFOITEM;
				QXmlStreamAttributes attributes = xConfig.attributes();
				if (attributes.hasAttribute("tag"))
				{
					itemInfo->sTag = attributes.value("tag").toString();
				}

				if (attributes.hasAttribute("format"))
				{
					itemInfo->sFormat = attributes.value("format").toString();
				}

				if (attributes.hasAttribute("row"))
				{
					itemInfo->iRow = attributes.value("row").toString().toInt();
				}
				
				if (attributes.hasAttribute("order"))
				{
					itemInfo->iOrder = attributes.value("order").toString().toInt();
				}
				
				if (attributes.hasAttribute("pos"))
				{
					QString sPos = attributes.value("pos").toString();
					modInfo->AddCornerInfo(sPos, *itemInfo);
				}
			}
		}
		else if (xConfig.isEndElement())
		{
			if (xConfig.name() == "Modality")
			{
				m_ModInfoV.push_back(*modInfo);
				delete modInfo;
				modInfo = NULL;
			}
			else if (xConfig.name() == "Element")
			{
				delete itemInfo;
				itemInfo = NULL;
			}
		}
	}
	return true;
}

bool CAppConfig::GetInfoSet(QString sModality, MODINFO& InfoSet)
{
	int n = int(m_ModInfoV.size());
	for (int i = 0; i < n; i++)
	{
		if (m_ModInfoV[i].sModality.compare(sModality) == 0 )
		{
			InfoSet = m_ModInfoV[i];
			return true;
		}
	}
	return false;
}
