#include"yaku.h"
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
	std::unordered_map<int,const std::string>::value_type(0x126,"��ȫ���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x207,"һ��ͨ��"),
	std::unordered_map<int,const std::string>::value_type(0x127,"һ��ͨ��"),
	std::unordered_map<int,const std::string>::value_type(0x208,"��ɫͬ˳"),
	std::unordered_map<int,const std::string>::value_type(0x128,"��ɫͬ˳"),
	std::unordered_map<int,const std::string>::value_type(0x310,"������"),
	std::unordered_map<int,const std::string>::value_type(0x300,"��ȫ���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x220,"��ȫ���۾�"),
	std::unordered_map<int,const std::string>::value_type(0x301,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0x221,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0x500,"��������"),
	std::unordered_map<int,const std::string>::value_type(0x600,"��һɫ"),
	std::unordered_map<int,const std::string>::value_type(0x520,"��һɫ"),
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
	const std::unordered_map<int, const std::string>::const_iterator iter = YakuName.find(index);
	if (iter != YakuName.end())
	{
		return iter->second;
	}
	return "";
}