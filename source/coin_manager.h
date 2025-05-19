#pragma once

#include "manager.h"
#include "coin_prop.h"

#include <vector>

class CoinManager : public SingleMgr<CoinManager>
{
	friend SingleMgr<CoinManager>;
public:
	typedef std::vector<CoinProp*> CoinPropList;

	void Increase_Coin(double num)
	{
        num_coin += num;
	}

	void Decrease_Coin(double num)
	{
        num_coin -= num;
		if (num_coin < 0)
            num_coin = 0;
	}

	void Update(double delta_time)
	{
		for (CoinProp* coin_prop : coin_Prop_List)
		{
            coin_prop->Update(delta_time);
		}
		coin_Prop_List.erase(
			std::remove_if
			(coin_Prop_List.begin(), coin_Prop_List.end(), 
				[](CoinProp * coin_prop) 
				{
					bool del = coin_prop->Can_Remove();
					if (del) delete coin_prop;
					return del;
				}
			), 
			coin_Prop_List.end());
	}

	void Render(SDL_Renderer* renderer)
	{
        for (CoinProp* coin_prop : coin_Prop_List)
            coin_prop->Render(renderer);
	}

	double Get_Current_Coin_Num()
	{
		return num_coin;
	}

	CoinPropList& Get_Coin_Prop_List()
	{
        return coin_Prop_List;
	}

	void Spawn_Coin_Prop(const Vector2& pos)
	{
		CoinProp* coin_prop = new CoinProp();
		coin_prop->Set_Position(pos);
        coin_Prop_List.push_back(coin_prop);

	}


protected:
	CoinManager()
	{
        num_coin = ConfigManager::GetInstance()->num_initial_coin;
	}
	~CoinManager()
	{
        for (CoinProp* coin_prop : coin_Prop_List)
            delete coin_prop;
	}

private:
	double num_coin = 0;

	CoinPropList coin_Prop_List;

};