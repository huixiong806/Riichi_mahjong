#pragma once
#include<unordered_map>
//��λ16����������һλΪ����(������d,����������f,ʳ���۰����������)���ڶ�λ:0=��ʳ����/ʳ���۵�����棬1=�����޶���2=ʳ���۵ķ������,����λΪ���
enum class Yaku
{
	Lizhi = 0x110,
	Yifa = 0x111,
	Menqianqingzimo = 0x112,
	Duanyaojiu = 0x100,
	Yipaizifeng = 0x101,
	Yipaichangfeng = 0x102,
	Yipaibai = 0x103,
	Yipaifa = 0x104,
	Yipaizhong = 0x105,
	Pinghu = 0x113,
	Yibeikou = 0x114,
	Qianggang = 0x106,
	Lingshangkaihua = 0x107,
	Haidilaoyue = 0x108,
	Hedilaoyu = 0x109,

	Lianglizhi = 0x210,
	Sansetongke = 0x200,
	Sangangzi = 0x201,
	Duiduihu = 0x202,
	Sananke = 0x203,
	Xiaosanyuan = 0x204,
	Hunlaotou = 0x205,
	Qiduizi = 0x211,

	Hunquandaiyaojiu = 0x206,
	HunquandaiyaojiuF = 0x226,
	Yiqitongguan = 0x207,
	YiqitongguanF = 0x227,
	Sansetongshun = 0x208,
	SansetongshunF = 0x228,

	Erbeikou = 0x310,
	Chunquantaiyaojiu = 0x300,
	ChunquantaiyaojiuF = 0x320,
	Hunyise = 0x301,
	HunyiseF = 0x321,

	Liujumanguan = 0x500,
	Qingyise = 0x600,
	QingyiseF = 0x620,


	Tianhu = 0xd00,
	Dihu = 0xd01,
	Dasanyuan = 0xd02,
	Sianke = 0xd10,
	Ziyise = 0xd03,
	Lvyise = 0xd04,
	Qinglaotou = 0xd05,
	Guoshiwushuang = 0xd11,
	Xiaosixi = 0xd06,
	Sigangzi = 0xd07,
	Jiulianbaodeng = 0xd12,

	Siankedanqi = 0xf10,
	Guoshiwushuangshisanmian = 0xf11,
	Chunzhengjiulianbaodeng = 0xf12,
	Dasixi = 0xf00,
};
const std::unordered_map<int, const std::string> YakuName =
{
	std::unordered_map<int,const std::string>::value_type(0x110,"��ֱ"),
	std::unordered_map<int,const std::string>::value_type(0x111,"һ��"),
	std::unordered_map<int,const std::string>::value_type(0x112,"��ǰ��������"),
	std::unordered_map<int,const std::string>::value_type(0x100,"���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x101,"���ƣ��Է���"),
	std::unordered_map<int,const std::string>::value_type(0x102,"���ƣ�������"),
	std::unordered_map<int,const std::string>::value_type(0x103,"���ƣ���"),
	std::unordered_map<int,const std::string>::value_type(0x104,"���ƣ���"),
	std::unordered_map<int,const std::string>::value_type(0x105,"���ƣ���"),
	std::unordered_map<int,const std::string>::value_type(0x113,"ƽ��"),
	std::unordered_map<int,const std::string>::value_type(0x114,"һ����"),
	std::unordered_map<int,const std::string>::value_type(0x107,"���Ͽ���"),
	std::unordered_map<int,const std::string>::value_type(0x108,"��������"),
	std::unordered_map<int,const std::string>::value_type(0x109,"�ӵ�����"),
	std::unordered_map<int,const std::string>::value_type(0x210,"����ֱ"),
	std::unordered_map<int,const std::string>::value_type(0x200,"��ɫͬ��"),
	std::unordered_map<int,const std::string>::value_type(0x201,"������"),
	std::unordered_map<int,const std::string>::value_type(0x202,"�ԶԺ�"),
	std::unordered_map<int,const std::string>::value_type(0x203,"������"),
	std::unordered_map<int,const std::string>::value_type(0x204,"С��Ԫ"),
	std::unordered_map<int,const std::string>::value_type(0x205,"����ͷ"),
	std::unordered_map<int,const std::string>::value_type(0x211,"�߶���"),
	std::unordered_map<int,const std::string>::value_type(0x206,"��ȫ���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x226,"��ȫ���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x207,"һ��ͨ��"),
	std::unordered_map<int,const std::string>::value_type(0x227,"һ��ͨ��"),
	std::unordered_map<int,const std::string>::value_type(0x208,"��ɫͬ˳"),
	std::unordered_map<int,const std::string>::value_type(0x228,"��ɫͬ˳"),
	std::unordered_map<int,const std::string>::value_type(0x310,"������"),
	std::unordered_map<int,const std::string>::value_type(0x300,"��ȫ���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x320,"��ȫ���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x301,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0x321,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0x500,"��������"),
	std::unordered_map<int,const std::string>::value_type(0x600,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0x620,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0xd00,"���"),
	std::unordered_map<int,const std::string>::value_type(0xd01,"�غ�"),
	std::unordered_map<int,const std::string>::value_type(0xd02,"����Ԫ"),
	std::unordered_map<int,const std::string>::value_type(0xd10,"�İ���"),
	std::unordered_map<int,const std::string>::value_type(0xd03,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0xd04,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0xd05,"����ͷ"),
	std::unordered_map<int,const std::string>::value_type(0xd11,"��ʿ��˫"),
	std::unordered_map<int,const std::string>::value_type(0xd06,"С��ϲ"),
	std::unordered_map<int,const std::string>::value_type(0xd12,"�ĸ���"),
	std::unordered_map<int,const std::string>::value_type(0xf10,"�İ��̵���"),
	std::unordered_map<int,const std::string>::value_type(0xf11,"��ʿ��˫ʮ����"),
	std::unordered_map<int,const std::string>::value_type(0xf12,"������������"),
	std::unordered_map<int,const std::string>::value_type(0xf00,"����ϲ"),
};
const std::string& getYakuName(int index)
{
	const std::unordered_map<int,const std::string>::const_iterator iter = YakuName.find(index);
	if (iter != YakuName.end())
	{
		return iter->second;
	}
	return "";
}