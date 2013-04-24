#include <map>
using namespace std;
class DayBaseAnalysis
{
public:
	static map<CString,int> dic_SearchEngineCount;//��¼ʹ����������Ĵ���(����->����)
	static map<CString,int> dic_SocailNetworkCount;//��¼ʹ���罻����Ĵ���(����-����)
	static char* SearchEngines[];	
	static char* SocailNetworks[];
	//Ҫ��Ҫ���strUrl�Ƿ��Ǳ�׼��ʽ��?
public:
	static void LoadTodayHistory(CString strFileName);//�ѵ�������ݼ��ص��ֵ���,����ǿ��������ʱ�ͼ���
	static void SaveTodayHistory(CString strFileName);//�Ѵ˴����ݴ洢���ļ���(����ǹر������ʱд�ļ�)������Ѿ��е������ݣ����޸�countֵ����
	static void AnalysisUrl(CString strUrl);//ʶ��url�Ƿ����罻�������������������
	static void Init();
	static bool IsSearchEngine(CString strUrl);
	static bool IsSocialNetWork(CString strUrl);
};