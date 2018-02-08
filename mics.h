
#include <matrix_hal/wishbone_bus.h>
#include <matrix_hal/microphone_array.h>

class Mics
{
    public:
    Mics(matrix_hal::WishboneBus& bus)
	{
        mics.Setup(&bus);
	}

    matrix_hal::MicrophoneArray& Object() { return mics; }
    
    private:
    matrix_hal::MicrophoneArray mics;
};
