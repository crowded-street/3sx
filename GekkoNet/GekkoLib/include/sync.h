#pragma once

#include <vector>

#include "gekko_types.h"
#include "input.h"

namespace Gekko {

	class SyncSystem {
	public:
		SyncSystem();

		void Init(u8 num_players, u32 input_size);

		void AddLocalInput(Handle player, u8* input);

		void AddRemoteInput(Handle player, u8* input, Frame frame);

		void IncrementFrame();

		bool GetCurrentInputs(std::unique_ptr<u8[]>& inputs, Frame& frame);

		bool GetSpectatorInputs(std::unique_ptr<u8[]>& inputs, Frame frame);

		bool GetLocalInputs(std::vector<Handle>& handles, std::unique_ptr<u8[]>& inputs, Frame frame);

		void SetLocalDelay(Handle player, u8 delay);
		
		u8 GetLocalDelay(Handle player);

		void SetInputPredictionWindow(Handle player, u8 input_window);

		Frame GetCurrentFrame();

		void SetCurrentFrame(Frame frame);

		Frame GetMinIncorrectFrame();

		Frame GetMinReceivedFrame();

        Frame GetLastReceivedFrom(Handle player);

	private:
		u8 _num_players;

		u32 _input_size;

		Frame _current_frame;

		std::unique_ptr<InputBuffer[]> _input_buffers;
	};
}
