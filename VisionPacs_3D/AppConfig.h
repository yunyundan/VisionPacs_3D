#pragma once
class CAppConfig
{
public:
	CAppConfig();
	~CAppConfig();

	//注释信息
	static vector<MODINFO> m_ModInfoV;

	bool GetBaseConfig();

	//从m_ModInfoV中找设置
	static bool GetInfoSet(QString sModality, MODINFO& InfoSet);
};

