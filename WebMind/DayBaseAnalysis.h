#include <map>
using namespace std;
class DayBaseAnalysis
{
public:
	static map<CString,int> dic_SearchEngineCount;//记录使用搜索引擎的次数(日期->次数)
	static map<CString,int> dic_SocailNetworkCount;//记录使用社交网络的次数(日期-次数)
	static char* SearchEngines[];	
	static char* SocailNetworks[];
	//要不要检查strUrl是否是标准形式呢?
public:
	static void LoadTodayHistory(CString strFileName);//把当天的数据加载到字典里,最好是开启浏览器时就加载
	static void SaveTodayHistory(CString strFileName);//把此次数据存储到文件中(最好是关闭浏览器时写文件)，如果已经有当天数据，则修改count值即可
	static void AnalysisUrl(CString strUrl);//识别url是否是社交还是搜索引擎或者其他
	static void Init();
	static bool IsSearchEngine(CString strUrl);
	static bool IsSocialNetWork(CString strUrl);
};