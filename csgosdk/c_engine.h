#pragma once
#include "../main.hpp"
#include "c_base_entity.h"

class c_engine
{
public:
	DWORD get_local_player()
	{
		return rpm<DWORD>(client + offsets::signatures::dwLocalPlayer);
	}

	DWORD get_entity_from_index(int idx)
	{
		return rpm<DWORD>(client + offsets::signatures::dwEntityList + (idx * 0x10));
	}

	DWORD get_engine_pointer()
	{
		return rpm<DWORD>(engine + offsets::signatures::dwClientState);
	}

	int get_max_players()
	{
		return rpm<int>(client_state + offsets::signatures::dwClientState_MaxPlayer);
	}

	map get_map_name()
	{
		return rpm<map>(client_state + offsets::signatures::dwClientState_Map);
	}

	bool is_in_game()
	{
		if (rpm<int>(get_engine_pointer() + offsets::signatures::dwClientState_State) == 6) {
			return true;
		}
		return false;
	}

	void force_jump()
	{
		wpm<int>(6, client + offsets::signatures::dwForceJump);
	}

	void force_attack()
	{
		wpm<int>(6, client + offsets::signatures::dwForceAttack);
	}

	void force_attack_secondary()
	{
		wpm<int>(6, client + offsets::signatures::dwForceAttack2);
	}

	void send_packets(bool state)
	{
		wpm<bool>(state, engine + offsets::signatures::dwbSendPackets);
	}

	void set_angles(vector angles)
	{
		wpm<vector>(angles, client_state + offsets::signatures::dwClientState_ViewAngles);
	}

	void glow_entity(c_base_entity entity, glow settings)
	{
		wpm<glow>(settings, glow_manager + ((entity.get_glow_index() * 0x38) + 0x4));
	}

};

extern c_engine g_engine;