#pragma once
#include"Single.hpp"
#include"Group.hpp"
#include<vector>
enum class ActionType
{
	Null,  //��ָ��
	Skip,
	Dapai, //��������
	Chi,
	Peng,
	Gang,  //��
	Rong,
	Lizhi,
	Zimo,
	Liuju,
	BaBei,
};
enum class ErrorType
{
	None,             //�ɹ��ˣ�û��error
	NotYourTurn,
	AlreadyActed,
	ActionRejected,
	TileNotExist,     //����ʱ������Ҫ��Ķ���
	CannotChooseNull, //��׼����ָ��
};
struct Action
{
	ActionType type;
	Single target;//��ʾ��������,������ʱ��Ч
	Group group;//����������Ч
	bool operator<(const Action& rhs)const
	{
		if ((int)type < (int)rhs.type)
			return true;
		if (target < rhs.target)return true;
		if (group < rhs.group)return true;
		return false;
	}
	Action() {
		type = ActionType::Null;
		target = Null;
	}
	Action(ActionType type_) {
		type = type_;
		target = Null;
		group = Group_Null;
	}
	Action(ActionType type_,Single target_,Group group_) {
		type = type_;
		target = target_;
		group = group_;
	}
	Action(ActionType type_, Single target_) {
		type = type_;
		target = target_;
	}
	Action(ActionType type_,Group group_) {
		type = type_;
		target = Null;
		group = group_;
	}
};
struct PlayerInfo
{
	std::vector<Group> groupTile;//��¶����
	std::vector<Single>discardTile;//�ƺ�
	int lizhi;//��ֱ���������ƺ��еĵڼ��ţ�-1Ϊδ��ֱ
	int lizhiXunmu;//��ֱ��ѲĿ
	int yifa;//�Ƿ���һ��Ѳ
	int score;
};
struct GameInfo
{
	Rule rule;//����
	std::vector<PlayerInfo>playerInfo;//��˳��ֱ�Ϊ���������ļ�
	WindType prevailingWind;   //����
	WindType selfWind;   //�ŷ�
	WindType nowWind;
	/*
	  ��ʾ�����(mingpai=true)/�ô���(mingpai=false)���˵��ŷ硣
	  ���Ϊ�Լ��Ļ���mingpai=true��ʾ�Լ�ʲôҲ�ɲ���(��Ϊ�������Լ�����)����Ҫ����ָ�
	  mingpai=false��ʾ�Լ��ô����ˣ�mingpai=false����������ΪNull˵���Ǹճԡ������ƺ�Ĵ��ơ�
	  ���Ϊ���˵Ļ���mingpai=trueʱ�Լ�����ѡ��һ��������mingpai=falseʱ��Ҫ����ָ��
	*/
	std::vector<Single>handTile;//����
	std::vector<Single>doraIndicator;//����ָʾ��
	Single nowTile;//�����ߵ������ƻ��ǿ�������/�ٺ͵Ķ���
	bool mingpai;//Ϊtrue��ʾ���˸մ����ƣ������ѡ�����ơ��ٺͻ�������
	bool lingshang;//��ǰ�������Ƿ�Ϊ������(��mingpai=falseʱ��Ч)
	bool moqie;//��ʾ���ȥ�������Ƿ�Ϊ����(��mingpai=trueʱ��Ч)
	int lizhibangCount;    //������ֱ������
	int benchang;    //������
	int subround; //Ѳ��
	int remainTiles;//ʣ������
	int round;//����(�綫2�־���2)
	bool w;//�Ƿ���w��,���,�غͿɳ�����״̬
};
class ActionGenerator
{
protected:
	std::string name;
public:
	virtual Action generateAction(const GameInfo& info)=0;
	virtual std::string getName()
	{
		return name;
	}
};
