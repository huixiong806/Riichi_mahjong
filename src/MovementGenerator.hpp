#pragma once
#include"Single.hpp"
#include"Group.hpp"
#include<vector>
enum class ActionType
{
	Null,  //��ָ��
	Skip,
	Chi,
	Peng,
	Gang,  //��
	Rong,
	Dapai, //��������
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
};
class ActionGenerator
{
protected:
	std::string name;
public:
	virtual Action generateAction(const GameInfo& info)=0;
};
