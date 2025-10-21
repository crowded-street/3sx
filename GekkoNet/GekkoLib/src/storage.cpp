#include "storage.h"

Gekko::StateStorage::StateStorage()
{
	_max_num_states = 0;
}


void Gekko::StateStorage::Init(u32 num_states, u32 state_size, bool limited)
{
	const u32 num = limited ? 2 : num_states + 2;
	_max_num_states = num;

	for (u32 i = 0; i < _max_num_states; i++) {
		_states.push_back(std::make_unique<StateEntry>());
        _states.back().get()->state = std::make_unique<u8[]>(state_size);
		_states.back().get()->state_len = state_size;
	}
}

Gekko::StateEntry* Gekko::StateStorage::GetState(Frame frame)
{
	frame = frame < 0 ? frame + _max_num_states : frame;
	return _states[frame % _max_num_states].get();
}
